To evaluate the sensitivity of muon radiography for planetary ice-density mapping, we developed a Geant4-based Monte Carlo simulation utilizing the FTFP_BERT physics list. The simulation environment consists of a 20 cm³ ice cube ($\rho = 0.917 \text{ g/cm}^3$) containing a $5 \times 5 \times 8$ cm iron inclusion. To replicate laboratory-scale muography, 10 cm $\times$ 10 cm $\times$ 5 cm Polyvinyl Toluene (PVT) scintillator plates were positioned above and below the target to serve as hodoscope elements.A synthetic cosmic-ray muon sky was modeled using a power-law energy spectrum ($dN/dE \propto E^{-2.7}$) across a 1–100 GeV range (mean energy $\approx$ 4 GeV), consistent with Gaisser’s parametrization (Gaisser 2004, PDG 2024). Primary muons were sampled via an inverse-transform of the cumulative distribution function (CDF) and initialized with a $\cos^2(\theta)$ angular distribution and randomized $(x, y)$ starting positions. This setup establishes an Earth-based sea-level baseline, allowing for direct comparison with experimental validation data before scaling to extraterrestrial flux environments.

This project uses Docker. Recommended setup is Docker+VsCode with the devcontainers extension, which will permit the user to reopen the project in vscode in the container, allowing for easy setup. 

Once that is done, follow these steps to run the project:

1. From the repository directroy, create a build folder and cd into it (mkdir build && cd build)
2. Run: cmake ..
3. Run: make 
Note: (optional) you can specify the number of cores (n) you want to allocate using the specifier -jn, e.g. -j4 to make the project compile faster
4. Run the simulation with: ./sim
[!IMPORTANT]Note on Data Volume: This file records step-level data and can grow quite large depending on your primary particle count. If you are only interested in specific regions, you can modify stepping.cc to filter for volumes (e.g., only recording hits within the Iron block) to minimize the output footprint.