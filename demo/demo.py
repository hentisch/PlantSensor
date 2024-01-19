import asyncio
from bleak import BleakClient
from time import sleep
import matplotlib.pyplot as plt
import numpy as np

addr = "34:85:18:7A:51:CD"

# plt.ion()
wavelengths = [415, 445, 580, 515, 555, 590, 630, 680]
async def main():
    async with BleakClient(addr) as client:
        while True:
            spectrum = await client.read_gatt_char("774c3ae8-f76a-4cb6-aaba-dcf19ce383fc")
            spectrum = "{0}".format("".join(map(chr, spectrum)))


            spectrum_array = [int(x) for x in spectrum.split(',')]
            plt.bar(range(8), spectrum_array, width=1, tick_label=[str(wavelength) + "nm" for wavelength in wavelengths], color=[wavelength_to_rgb(wavelength) for wavelength in wavelengths])
            plt.annotate("43C", (10, 10))
            plt.draw()
            plt.pause(0.0001)
            plt.clf()

def wavelength_to_rgb(wavelength, gamma=0.8):

    '''This converts a given wavelength of light to an 
    approximate RGB color value. The wavelength must be given
    in nanometers in the range from 380 nm through 750 nm
    (789 THz through 400 THz).
    Based on code by Dan Bruton
    http://www.physics.sfasu.edu/astro/color/spectra.html
    '''

    wavelength = float(wavelength)
    if wavelength >= 380 and wavelength <= 440:
        attenuation = 0.3 + 0.7 * (wavelength - 380) / (440 - 380)
        R = ((-(wavelength - 440) / (440 - 380)) * attenuation) ** gamma
        G = 0.0
        B = (1.0 * attenuation) ** gamma
    elif wavelength >= 440 and wavelength <= 490:
        R = 0.0
        G = ((wavelength - 440) / (490 - 440)) ** gamma
        B = 1.0
    elif wavelength >= 490 and wavelength <= 510:
        R = 0.0
        G = 1.0
        B = (-(wavelength - 510) / (510 - 490)) ** gamma
    elif wavelength >= 510 and wavelength <= 580:
        R = ((wavelength - 510) / (580 - 510)) ** gamma
        G = 1.0
        B = 0.0
    elif wavelength >= 580 and wavelength <= 645:
        R = 1.0
        G = (-(wavelength - 645) / (645 - 580)) ** gamma
        B = 0.0
    elif wavelength >= 645 and wavelength <= 750:
        attenuation = 0.3 + 0.7 * (750 - wavelength) / (750 - 645)
        R = (1.0 * attenuation) ** gamma
        G = 0.0
        B = 0.0
    else:
        R = 0.0
        G = 0.0
        B = 0.0
    return ((R), (G), (B))

asyncio.run(main())