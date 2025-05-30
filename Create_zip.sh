!/bin/bash
mkdir -p tinyProject1
cp Vector.hpp Vector.cpp Matrix.hpp Matrix.cpp LinearSystem.hpp LinearSystem.cpp main.cpp tinyProject1/
cd tinyProject1
zip -r tinyProject1.zip .
mv tinyProject1.zip ..
cd ..
rm -rf tinyProject1