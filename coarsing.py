import numpy as np
from scipy.stats import binned_statistic

def pdfize(vector: list) -> np.array:
    """
    Estimate the probaility density function of a vector
    :param vector: list of numbers
    """
    pdf, _ = np.histogram(vector, bins=np.logspace(np.log10(min(vector)), np.log10(max(vector)), round(len(vector)/50)))
    return pdf / pdf.sum()

def coarse(f):
    """
    function to coarse a vector

    If a number is present in a bin it replaces the whole bin with its mean
    """
    def c(vec):
        return vec.mean() if len(vec) > 0 else np.nan

    def wrapper(*args, **kwargs):
        return f(function = c, *args, **kwargs)
    return wrapper

@coarse
def coarse_vec(vector: np.array, window = 4, function = lambda x: x.sum()) -> np.array:
    """
    Function to apply coarsing to a vector

    :param vector: vector to coarse
    :param window: window's shape
    :param function: function for coarsing, can be passed as decorator 
    """
    data = np.array(vector)
    bin_stat, bin_edges, _ = binned_statistic(np.arange(0, len(data), 1), data, statistic=function, bins= np.linspace(0, len(data), int(round(len(data)/window))))
    return bin_stat[~np.isnan(bin_stat)]

if __name__ == "__main__":
    print("This is a module")