import os
import subprocess

from muteria.configmanager.configurations import SessionMode
from muteria.configmanager.configurations import TestcaseToolsConfig
from muteria.configmanager.configurations import CriteriaToolsConfig
from muteria.configmanager.configurations import ToolUserCustom
from muteria.drivers.testgeneration import TestToolType
from muteria.drivers.criteria import CriteriaToolType
from muteria.drivers.criteria import TestCriteria
from muteria.drivers import DriversUtils

from muteria.drivers.testgeneration.tools_by_languages.c.semu.\
                                        driver_config import DriverConfigSemu

#from muteria.repositoryandcode.build_utils.c import make_build_func

from muteria.common.mix import GlobalConstants

#from muteria.drivers.testgeneration.testcase_formats.system_devtest import \
#                                                            system_test_runner
#from muteria.drivers.testgeneration.custom_dev_testcase.system_wrappers.\
#                                            native_code import SystemWrapper
import muteria.drivers.testgeneration.tools_by_languages.c.semu.semu \
                                                                as semu_module


PROGRAMMING_LANGUAGE='C'
REPOSITORY_ROOT_DIR="{{ template_repo_rootdir }}"
OUTPUT_ROOT_DIR="{{ template_output_dir }}"
RUN_MODE=SessionMode.EXECUTE_MODE

TARGET_SOURCE_INTERMEDIATE_CODE_MAP = {'{{ template_programe_name }}.c':'{{ template_programe_name }}.o'}
REPO_EXECUTABLE_RELATIVE_PATHS = ["{{ template_programe_name }}"]

if not os.path.isdir(REPOSITORY_ROOT_DIR):
    os.mkdir(REPOSITORY_ROOT_DIR)
    open(os.path.join(REPOSITORY_ROOT_DIR, '{{ template_programe_name }}.c'), "w").close()
    open(os.path.join(REPOSITORY_ROOT_DIR, '{{ template_programe_name }}.o'), "w").close()
    open(os.path.join(REPOSITORY_ROOT_DIR, '{{ template_programe_name }}'), "w").close()

CODE_BUILDER_FUNCTION = lambda *args, **kwargs: None

CUSTOM_DEV_TEST_RUNNER_FUNCTION = lambda *args, **kwargs: None
#CUSTOM_DEV_TEST_PROGRAM_WRAPPER_CLASS = SystemWrapper
#DEVELOPER_TESTS_LIST = []

# custom devtest
#dev_test = TestcaseToolsConfig(tooltype=TestToolType.USE_ONLY_CODE, toolname='custom_devtests', config_id=0)
#dev_test.set_one_test_execution_timeout(2)
#dev_test.set_test_oracle_test(True)


# semu tests
semu_config_args = [
                                ('--write-kqueries',),
                                ('--write-smt2s',),
                                ('-max-memory', '{{ template_memory_limit }}'),
                                
                                #('-semu-no-state-difference',),
                                #('-semu-MDO-propagation-selection-strategy',),
                                #('-semu-use-basicblock-for-distance',),
                                ('-semu-forkprocessfor-segv-externalcalls',),
                                #('-semu-testsgen-only-for-critical-diffs',),
                                #('-semu-no-environment-output-diff',),
                                ('--semu-use-only-multi-branching-for-depth',),

                                ('-semu-checkpoint-window', '{{ template_CW }}'),
                                ('-semu-minimum-propagation-depth', '{{ template_MPD }}'),
                                ('-semu-propagation-proportion', '{{ template_PP }}'),
                                ('-semu-precondition-length', '{{ template_PL }}'), # start from top
                                #('-semu-max-total-tests-gen', '1000')
                                ('-semu-number-of-tests-per-mutant', '{{ template_NTPM }}'),
]

if {{ template_candidate_mutants_list }} is not None:
    semu_config_args.append(('-semu-candidate-mutants-list-file', {{ template_candidate_mutants_list }}))
if {{ template_disable_post_mutation_check }}:
    semu_config_args.append(("--semu-disable-post-mutation-check",))
if not {{ template_stop_on_memory_limit }}:
    semu_config_args.append(('--semu-no-error-on-memory-limit',))
if not {{ template_max_memory_inhibit }}:
    semu_config_args.append(('--max-memory-inhibit',))
                            
semu_test = TestcaseToolsConfig(tooltype=TestToolType.USE_CODE_AND_TESTS, toolname='semu', \
                        tool_user_custom=ToolUserCustom(
                            PRE_TARGET_CMD_ORDERED_FLAGS_LIST=semu_config_args, 
                            POST_TARGET_CMD_ORDERED_FLAGS_LIST={{ template_sym_args_list_of_lists }},
                            DRIVER_CONFIG = DriverConfigSemu(meta_mutant_source="{{ template_meta_mu_bc_file }}", \
                                                                     verbose_generation=True, \
                                                                     suppress_generation_stdout=False),
                        )
                    )
semu_test.set_test_gen_maxtime({{ template_test_gen_maxtime }})

# test tool list
TESTCASE_TOOLS_CONFIGS = [
        semu_test,
]

#ENABLED_CRITERIA = [
#        TestCriteria.STRONG_MUTATION,
#]

#mart = CriteriaToolsConfig(tooltype=CriteriaToolType.USE_ONLY_CODE, toolname='mart', config_id=0)
#CRITERIA_TOOLS_CONFIGS_BY_CRITERIA[TestCriteria.STRONG_MUTATION] = [mart] 

LOG_DEBUG = True

#DISCARD_FLAKY_TESTS = False
