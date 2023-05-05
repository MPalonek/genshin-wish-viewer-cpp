https://docs.opencv.org/4.x/d3/d52/tutorial_windows_install.html#tutorial_windows_install_path

Installing OpenCV
- unpack opencv-4.6.0
- run terminal
- setx OpenCV_DIR D:\Repo\c++\genshin-wish-viewer-cpp\importer\opencv-4.6.0\build
- restart computer

https://github.com/Microsoft/vcpkg/blob/master/README.md#quick-start-windows
https://tesseract-ocr.github.io/
https://learn.microsoft.com/en-us/vcpkg/

Packages (vcpkg)
- install vcpkg
	> git clone https://github.com/microsoft/vcpkg
	> vcpkg\bootstrap-vcpkg.bat
- integrate vcpkg globally
	> vcpkg integrate install
- install tesseract
	> vcpkg install tesseract:x64-windows-static
- install opencv
	> vcpkg install opencv:x64-windows-static