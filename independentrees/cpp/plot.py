import plotly.graph_objects as go
from scipy.optimize import curve_fit
import numpy as np

data = np.genfromtxt("data.csv")

#cnts = np.random.choice(data[data > 0], size=50000, replace=False)
cnts = np.abs(data)
cnts = np.sort(cnts)[::-1]
cnts = cnts/np.sum(cnts)

# cnts1 = np.random.choice(data, size=50000, replace=False)
# cnts1 = np.abs(cnts1)
# cnts1 = np.sort(cnts1)[::-1]
# cnts1 = cnts1/np.sum(cnts1)

x = np.linspace(1, 1+len(cnts), len(cnts))


fit_func = lambda x, C, alpha: C*np.power(x,alpha)

popt, pcov = curve_fit(fit_func, x, cnts, p0=(0.1,-1.5), bounds=([0.0001, -3],[0.1, -0.1]))

fig = go.Figure()
fig.add_traces([
    go.Scatter(x=x, y=cnts, line={"width":5}, name="data"),
    # go.Scatter(x=x, y=cnts1, line={"width":5}, name="data"),
    go.Scatter(x = np.linspace(1, 1+len(data)), 
               y = fit_func(np.linspace(1, 1+len(data)), *popt), 
               #y = 1e-1/np.linspace(1, 1+len(data)),
               line={"width": 5, "dash": "dash"}, name=f"x^{round(popt[1],2)}"),
    go.Scatter(x=np.linspace(1, 1+len(data)),
               y = 1e-1/np.linspace(1, 1+len(data)),
               line={"width": 5, "dash": "dash", "color":"gray"}, name=f"1/r")
])

fig.update_layout({
    "xaxis": {"type": "log", "title": "rank, r"},
    "yaxis": {"type":"log", "title": "abs(leaf) / sum(leaves)"},
})

fig.show()
fig.write_image("independent_norm.pdf", engine="kaleido")
