import sys

def process_csv(results_path, file_to_process):
    times = {}
    with open(results_path + file_to_process) as f:
        for line in f:
            line_fields = line.split(';')
            times[line_fields[0]] = line_fields[1]
    return times

def calculate_diff(final_file_path, times_original, times_prioritized):
    file_path = open(final_file_path, 'w')
    for key, value in times_prioritized.items():
        decrease = (int(times_original[key]) - int(value)) / int(times_original[key])
        
        file_path.write(key + ';' + str(decrease)  +'\n')

results_path = str(sys.argv[1])

times = process_csv(results_path, "/nr_tests.csv")
s1jaccard = process_csv(results_path, "/s1jaccard.csv")
s1ochiai = process_csv(results_path, "/s1ochiai.csv")
s2cosine = process_csv(results_path, "/s2cosine.csv")
s2euclidean = process_csv(results_path, "/s2euclidean.csv")

calculate_diff(results_path + '/s1jaccard_decreases.csv', times, s1jaccard)
calculate_diff(results_path + '/s1ochiai_decreases.csv', times, s1ochiai)
calculate_diff(results_path + '/s2cosine_decreases.csv', times, s2cosine)
calculate_diff(results_path + '/s2euclidean_decreases.csv', times, s2euclidean)

