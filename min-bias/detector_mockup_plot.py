
import matplotlib.patches as mpatches
from matplotlib.collections import PatchCollection


Barrel = [
    {'name': 'Vacuum',
     'rmax': 20,
     'width': 8000,
     'color': ''  },
     {'name': 'Beampipe',
     'rmax': 21,
     'width': 8000,
     'color': 'grey'},
     {'name': 'Tracker',
     'rmax': 2500,
     'width': 8000,
     'color': 'red'},
     {'name': 'EMCAL',
     'rmax': 3600,
     'width': 8000,
     'color': 'blue'},
     {'name': 'HCAL',
     'rmax': 6000,
     'width': 8000,
     'color': 'green'},
     {'name': 'Coil1',
     'rmax': 7825,
     'width': 10100,
     'color': 'black'},
     {'name': 'Muon',
     'rmax': 13000,
     'width': 11500,
     'color': 'orange'},
     {'name': 'Coil2',
     'rmax': 13470,
     'width': 7600,
     'color': 'black'}
     ]

Forward = [
    {'name': 'FMuon',
     'rmin': 21,
     'rmax': 5000,
     'zmin': 27500,
     'zmax': 31500,
     'color': 'orange'} ]



def getBarrel(): 
    twin_solenoid_barrel = {}
    for previous, current in zip(Barrel[:-1], Barrel[1:]):
        print current
        twin_solenoid_barrel[current['name']] = mpatches.Rectangle([0, previous['rmax']], 
            current['width'], 
            current['rmax'] - previous['rmax'], 
            color=current['color']) 
    return twin_solenoid_barrel


def getForward():
    twin_solenoid_forward = {}
    for current in Forward:
        twin_solenoid_forward[current['name']] = mpatches.Rectangle([current['zmin'], current['rmin']],
            current['zmax'] - current['zmin'],
            current['rmax'] - current['rmin'],
            color = current['color'])
    return twin_solenoid_forward


def plot_fcc_envelopes(ax):
  for p in getBarrel().values():
      ax.add_artist(p)
  for p in getForward().values():
      ax.add_artist(p)
  ax.set_xlim(1e-6, 30000)
  ax.set_ylim(1e-6, 20000)
