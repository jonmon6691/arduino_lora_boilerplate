mkdir bootloaders
git clone https://github.com/Optiboot/optiboot.git
cd optiboot
git checkout 6aa5871762a5f81942e41245c53415753464573b
git apply ../bootloader_mod.patch
cd optiboot/bootloaders/optiboot && ./makeall.arduino.sh
cp *.hex ../../../../bootloaders
echo
echo
echo "[Done] Check the variables in burn_bootloader.sh then run it to burn the bootloader"
echo

