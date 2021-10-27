
Run the tests with the command:
```
./runtests.sh
```

This will run using the `SEMu` docker image if SEMu is not installed.

In order to run within the `faqas_semu` docker image, use the following command (after building the `faqa_semu` docker image):
```
USE_FAQAS_SEMU_DOCKER_IMAGE=ON ./runtests.sh
```
