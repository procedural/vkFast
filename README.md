vkFast
------

A full-blown step-by-step video tutorial on how to use vkFast is here! 😅 Go watch it!

https://www.youtube.com/watch?v=XYz3erNX5VM

<img width="716" height="477" alt="thumb" src="https://github.com/user-attachments/assets/d3780970-0d82-4949-8b84-2cbb03c9ba04" />

Another video tutorial on how to printf in GPU compute shaders in Vulkan vkFast on Linux:

https://www.youtube.com/watch?v=mlU35k1CufY

The examples are tested on Intel i3 12100 CPU and Nvidia RTX 2060 (Turing) GPU, AMD Radeon Pro V520 (RDNA 1.0) GPU, AMD Radeon RX 550 (GCN 4.0, 2gb model) GPU, Intel Arc B580 GPU and Intel UHD Graphics 730 iGPU.

Install instructions for Ubuntu
-------------------------------

First, open a terminal and run:
```sh
sudo apt install build-essential procps curl file git
```

Then go to https://brew.sh/ and install Homebrew.

DO NOT CLOSE THE TERMINAL when brew will be finished installing! You'll see similar to the following commands that you need to run:
```sh
echo >> ~/.bashrc
echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv bash)"' >> ~/.bashrc
eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv bash)"
```

Run `brew help` to see if Homebrew was installed successfully.

Then run the following command to make `/home/linuxbrew/` folder writable:
```sh
sudo chown -R $(whoami):$(id -g) /home/linuxbrew
```

You also need the latest REDGPU SDK placed in `/home/linuxbrew/` folder, you can do it like so:
```sh
git clone https://github.com/redgpu/redgpu --depth 1
git clone https://github.com/redgpu/redgpu2 --depth 1
cp -r redgpu/RedGpuSDK/ /home/linuxbrew/
cp -r redgpu2/RedGpuSDK/ /home/linuxbrew/
```

To compile and run the `examples/00 Hello Compute/main.c` file, install these dependencies with brew:
```sh
brew install libx11 vulkan-loader
```
