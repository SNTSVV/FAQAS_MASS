import os
import subprocess

class MASSSampling:

    cwd = os.path.dirname(os.path.abspath(__file__))
    fsci_stopping_script = os.path.join(cwd, "fsci_stopping_criterion.R")
    fsci_prt_stopping_script = os.path.join(cwd, "fsci_prt_stopping_criterion.R")
    fsci_error_script = os.path.join(cwd, "fsci_compute_error.R")
    get_ci_script = os.path.join(cwd, "get_ci.R")
    get_corrected_ci_script = os.path.join(cwd, "get_corrected_ci.R")

    tolerated_error = 0.035
    fsci_calibration = 150
    delta = 1.0
    higher = 0.0
    lower = 0.0

    def __init__(self, results_mutant_file, sampling, total_mutants, reduced):
        self.results_mutant_file = results_mutant_file
        self.sampling = sampling
        self.total_mutants = total_mutants
        self.reduced = reduced

    def fsci_stopping_criterion(self):
        result = subprocess.run([self.fsci_stopping_script, self.results_mutants_file], stdout=subprocess.PIPE).stdout.decode('utf-8')
        print("stopping criterion is", result)
        return int(result)

    def fsci_prt_stopping_criterion(self):
        result = subprocess.run([self.fsci_prt_stopping_script, self.results_mutants_file, str(self.delta), str(self.lower), str(self.higher)], stdout=subprocess.PIPE).stdout.decode('utf-8')
        print("stopping criterion is", result)
        return int(result)

    def get_ci(self):
        result = subprocess.run([self.get_ci_script, self.results_mutants_file], stdout = subprocess.PIPE).stdout.decode('utf-8')
        return result

    def get_corrected_ci(self):
        result = subprocess.run([self.get_corrected_ci_script, self.results_mutants_file, str(self.delta), str(self.lower), str(self.higher)], stdout = subprocess.PIPE).stdout.decode('utf-8')
        return result

    def estimate_error(self):
        result = subprocess.run([self.fsci_error_script, error_mutants_file, self.results_mutants_file], stdout=subprocess.PIPE).stdout.decode('utf-8')
        return result

    def stopping_criterion(self, count):
        if self.sampling == "fsci":

            if count == self.total_mutants:
                confidence_interval = self.get_corrected_ci() if self.reduced else self.get_ci()
                print("Confidence interval", confidence_interval)

            if self.reduced:
                if count < self.fsci_calibration:
                    return 0
                else:
                    return self.fsci_prt_stopping_criterion() if self.delta < self.tolerated_error else self.fsci_stopping_criterion()
            else:
                return self.fsci_stopping_criterion()

        else:
            # we covered all sampled mutants
            if count == self.total_mutants:
                return 1
            else:
                return 0

    def setFsciError(self):
        error_prt = self.estimate_error()
        error_prt_split = error_prt.split(";")
        self.delta = float(error_prt_split[0])
        self.lower = float(error_prt_split[1])
        self.higher = float(error_prt_split[2])

        print("delta", self.delta, "lower", self.lower, "higher", self.higher)
