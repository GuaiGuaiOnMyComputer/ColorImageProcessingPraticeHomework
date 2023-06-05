import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FormatStrFormatter
from os import walk
from os.path import join, splitext, isfile

TEST_SIZE = 5000
IMG_SHAPES = ((800, 600), 
              (1024, 768), 
              (1280, 1024), 
              (1920, 1080),
              (2000, 2500),
              (4000, 3000),
              (5000, 3500))

def load_txt_stats() -> np.ndarray:
    stats = np.zeros((TEST_SIZE, 7, 2), dtype = np.double)
    for (root, dirs, files) in walk(join("FinalPresentation", "experiment", "log", "convolutiontime")):
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
                case "108":
                    test_type = 3
                case "250":
                    test_type = 4
                case "300":
                    test_type = 5
                case "350":
                    test_type = 6
            match file_name[:2]:
                case "my":
                    mat_type = 0
                case "cv":
                    mat_type = 1

            stats[:, test_type, mat_type] = np.loadtxt(join(root, file_name), delimiter = ',', dtype = np.double)
    np.save(join("FinalPresentation", "experiment", "log", "convolutiontime", "stats_convolution_time.npy"), stats)
    return stats

def remove_outliners(stats):
    standard_deviations = np.std(stats, 0)
    averages = np.mean(stats, 0)
    stats_without_outliners = np.zeros_like(stats)
    none_outliner_count = np.zeros((2, 7), np.int32)
    for group in range(2):
        for shape_opt in range(7):
            stats_without_outliners[:, shape_opt, group] = np.where(stats[:, shape_opt, group] - averages[shape_opt, group] > 4 * standard_deviations[shape_opt, group], averages[shape_opt, group], stats[:, shape_opt, group])
    none_outliner_count = np.sum(stats_without_outliners > 0, 0)
    return stats_without_outliners

def get_avg_time(stats):
    avg = np.zeros((7, 2), dtype = np.double)
    for group in range(2):
        for shape_opt in range(7):
            for sample in range(1000):
                if stats[sample, shape_opt, group] > 0:
                    avg[shape_opt, group] += stats[sample, shape_opt, group]
    avg /= 5000
    return avg


def plot_curve(data):
    plt.plot(data[:, 0], label = "my::Mat", marker = "o")
    plt.plot(data[:, 1], label = "cv::Mat", marker = "p")
    plt.legend()
    plt.xticks([_ for _ in range(len(IMG_SHAPES))], IMG_SHAPES)
    plt.xlabel("Image Shapes")
    plt.gca().yaxis.set_major_formatter(FormatStrFormatter("%.3f"))
    plt.ylabel("Convolution Time (sec)")
    plt.title("Mat Convolution Time of 7 Image Shapes")
    plt.show()


def main():
    stats = load_txt_stats()
    stats_without_outliners = remove_outliners(stats)
    average_time = get_avg_time(stats_without_outliners)
    plot_curve(average_time)

if __name__ == "__main__":
    main()