@page developer_zone_page Developer Zone

Add conda to path
https://www.geeksforgeeks.org/how-to-setup-anaconda-path-to-environment-variable/
C:\Users\dani0\anaconda3\Scripts

Create python env:
conda create -n "shellminator" python=3.9 --file .\extras\requirements.txt

Activate environment
conda activate shellminator

Install packages that are not available on CondaForge
pip install minify-html

Generate examples
python .\exampleGenerator.py

Build examples for PC
python build.py -t "clean examples"

BLUETOOTH

chrome://flags/#enable-experimental-web-platform-features -> Enable Experimental Web Platform features
https://reelyactive.github.io/web-ble-demo/ -> tutorial