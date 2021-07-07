#
# Build command (from folder containing Dockerfile):
#>  git clone https://gitlab.uni.lu/ocornejo1/srcirorfaqas.git srcirorfaqas && sudo docker build --no-cache -t faqas_semu . && rm -rf srcirorfaqas
#

# build llvm-3.8 and MASS
FROM ubuntu:20.04 AS builder

COPY . /home/FAQAS/faqas_semu

RUN test -d /home/FAQAS/faqas_semu/srcirorfaqas || { \
    printf "\n# ERROR: first clone srcirorfaqas into faqas_semu repo as following:\n"; \
    printf "\t git clone https://gitlab.uni.lu/ocornejo1/srcirorfaqas.git srcirorfaqas\n"; \
    printf "\t\t OR\n"; \
    printf "\t git clone ssh://git@gitlab.uni.lu:8022/ocornejo1/srcirorfaqas.git\n\n"; \
    exit 1; }

RUN apt-get -y update; exit 0
RUN apt-get -y install cmake clang lib32z1-dev \
  && apt-get install -y python3-pip python3-dev \
  && cd /usr/local/bin \
  && ln -s /usr/bin/python3 python \
  && cd - \
  && ln -s $(which pip3) $(dirname $(which pip3))/pip

RUN mv /home/FAQAS/faqas_semu/srcirorfaqas /home/FAQAS/MASS 
RUN cd /home/FAQAS/MASS && bash llvm-build.sh 

FROM thierrytct/klee-semu:llvm-3.4.2

ENV SRCIROR_LLVMMutate_LIB=/home/FAQAS/MAAS/llvm-build/Release+Asserts/lib/LLVMMutate.so
ENV SRCIROR_SRC_MUTATOR=/home/FAQAS/MAAS/SRCMutation/build/mutator
ENV SRCIROR_LLVM_BIN=/home/FAQAS/MAAS/llvm-build/Release+Asserts/bin/
ENV SRCIROR_LLVM_INCLUDES=/home/FAQAS/MAAS/llvm-build/Release+Asserts/lib/clang/3.8.0/include/
ENV C_INCLUDE_PATH=${C_INCLUDE_PATH}:/home/klee-semu/klee_src/include/

COPY --from=builder /home/FAQAS/faqas_semu /home/FAQAS/faqas_semu
COPY --from=builder /home/FAQAS/MASS /home/FAQAS/MASS

RUN apt-get -y update \
    && bash /home/FAQAS/faqas_semu/install_requirements.sh

CMD echo "# `faqas_semu` is located in the directory '/home/FAQAS/faqas_semu'. For the ASN case study, make sure to mount the WORKSPACE folder. (use the script /home/FAQAS/faqas_semu/case_studies/ASN/scripts/docker_run.sh)"
