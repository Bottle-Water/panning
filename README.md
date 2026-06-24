# Panning Plugin
JUCE audio plugin implementing three stereo panning laws.

### Overview
This plugin applies one of three panning laws to an audio input. A slider controls the pan position and a combo box selects the panning algorithm.


https://github.com/user-attachments/assets/c453739f-2d2c-4c62-8939-daa94d63b83f


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
Build in your IDE

### Steps Report
1. Created the Projucer project as an audio plugin, named it Panning, set it as audio to audio.
2. Implemented the three panning laws in processBlock. (see comments in code for formulas)
3. Made the editor UI: slider for pan, combo box for law selection.
4. Built and tested in VST3 in JUCE's `audiopluginhost`.

### Screenshots
<img width="385" height="294" alt="-4 5dBPan" src="https://github.com/user-attachments/assets/5f2f041e-784f-4e17-bf82-4c7a8ba2861c" />
<img width="386" height="294" alt="LinearPan" src="https://github.com/user-attachments/assets/211875ae-95bc-45ad-8b2a-8bdb3b67325c" />
<img width="390" height="293" alt="ConstantPowerPan" src="https://github.com/user-attachments/assets/c5e96574-6184-42a1-bbfd-8c9527b8f3f8" />

