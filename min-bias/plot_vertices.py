from EventStore import EventStore
import numpy as np
import matplotlib.pyplot as plt
from detector_mockup_plot import plot_fcc_envelopes
import glob


def parse_filename(f):
  return f.split('_')[2].split('GeV')[0]

for f in glob.glob('*.root'):
    print f
    events = EventStore(f)
    print 'processing ', len(events), ' from ', f, '...'
    pos = []
    for store in events:
        clusters = store.get('GenVertex')
        for c in clusters:
            cor = c.Position()
            pos.append([cor.X, cor.Y, cor.Z])
    pos = np.array(pos)
    plt.figure('VertexHistZ')
    plt.hist(pos[:,2], log=True, histtype='step', bins=np.logspace(-5,7,100), 
        lw=1.5, label=parse_filename(f))
    plt.figure()
    ax = plt.axes()
    plot_fcc_envelopes(ax)
    ax.plot(np.abs(pos[:,2]), np.sqrt(pos[:,0]**2 + pos[:,1]**2), 'o', 
    alpha=0.5, label=parse_filename(f))
    plt.xscale('log')
    plt.xlabel('GenVertex Z-Position ')
    plt.yscale('log')
    plt.ylabel('GenVertex R-Position')
    plt.savefig(f.replace('.root', '') + 'vertices-R_versus_Z.png')

plt.figure('VertexHistZ')
plt.xscale('log')
plt.legend(title='Pythia8 minbias, N=3000 events', loc='best')
plt.savefig('VertexHistZ.png')

