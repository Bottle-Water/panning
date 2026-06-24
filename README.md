# Panning Plugin
JUCE audio plugin implementing three stereo panning laws.

### Overview
This plugin applies one of three panning laws to an audio input. A slider controls the pan position and a combo box selects the panning algorithm.

### Panning Laws


#### Linear Panning

Left and right gains sum to 1. Creates a "hole in the middle" effect where the perceived loudness drops at the center position by approximately -3dB in power terms. Typically, it is not amplitude that is additive (as seen here), but instead power, leading to this “hole”.

#### Constant Power Panning
Replaces the linear gains with cosine and sine functions. Since cos<sup>2</sup>θ + sin<sup>2</sup>θ = 1 (power is amplitude<sup>2</sup>), total power is preserved across all pan positions, eliminating the "hole in the middle" effect.


#### -4.5dB Pan Law
A compromise between the first two, the square root of the product of the linear and constant power gains.

### Building

Open `Panning.jucer` in Projucer
Select your exporter and click `Save and Open in IDE`
Build in your IDE.