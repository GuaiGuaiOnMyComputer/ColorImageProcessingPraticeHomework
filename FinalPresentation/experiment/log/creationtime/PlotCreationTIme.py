import numpy as np
import matplotlib.pyplot as plt
from os import walk
from numba import njit
from os.path import dirname, join, splitext

TEST_SIZE = 5000
def load_txt_stats() -> np.ndarray:
    stats = np.zeros((TEST_SIZE, 3, 2), dtype = np.double)
    for (root, dirs, files) in walk(dirname(__file__)):
        for file_name in files:
            test_type = -1
            mat_type = -1
            if splitext(file_name)[-1] != ".txt":
                continue

            match file_name[8:11]:
                case "600":
                    test_type = 0
                case "768":
                    test_type = 1
                case "102":
                    test_type = 2
            match file_name[:2]:
                case "my":
                    mat_type = 0
                case "cv":
                    mat_type = 1

            stats[:, test_type, mat_type] = np.loadtxt(join(root, file_name), delimiter = ',', dtype = np.double)
    return stats

def remove_outliners(stats):
    standard_deviations = np.std(stats, 0)
    averages = np.mean(stats, 0)
    stats_without_outliners = np.zeros_like(stats)
    none_outliner_count = np.zeros((3, 2), np.int)
    for group in range(2):
        for shape_opt in range(3):
            stats_without_outliners[:, shape_opt, group] = np.where(stats[:, shape_opt, group] - averages[shape_opt, group] > 4 * standard_deviations[shape_opt, group], -1000, stats[:, shape_opt, group])
    none_outliner_count = np.sum(stats_without_outliners > 0, 0)
    return stats_without_outliners, none_outliner_count

def get_avg_time(stats, none_outliner_count):
    avg = np.zeros((3, 2), dtype = np.double)
    for group in range(2):
        for shape_opt in range(3):
            for sample in range(5000):
                if stats[sample, shape_opt, group] > 0:
                    avg[shape_opt, group] += stats[sample, shape_opt, group]
    avg /= none_outliner_count
    return avg

def main():
    stats = load_txt_stats()
    stats_without_outliners, none_outliner_count = remove_outliners(stats)
    average_time = get_avg_time(stats_without_outliners, none_outliner_count)

    pass

if __name__ == "__main__":
    main()