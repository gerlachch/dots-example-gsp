# dots-example-gsp

The *dots-example-gsp* is a set of example services for the DOTS IPC system (see [dots-cpp](https://github.com/pnxs/dots-cpp)).

# TL;DR

1. Clone latest revision. 
2. Build with CMake.
3. Execute the `run.ps1` (Windows) or `run.sh` (Linux) scripts respectively to start the services.
4. (Optional) Use the [dots-explorer](https://github.com/gerlachch/dots-explorer) to inspect and manipulate objects.

# Overview

The dots-example-gsp contains a set of applications that demonstrate how a combined set of features can be implemented with DOTS based services. 

# Runtime Dependencies

* The OpenGL runtime (by default should be installed on most systems).

# Build Dependencies

* All dependencies of [dots-cpp](https://github.com/pnxs/dots-cpp#dependencies) (but not dots-cpp itself!).
* OpenGL SDK.
* C++ compiler supporting at least C++20 (such as GCC 9.3 or MSVC 19.14).

# Build and Run

This is how the dots-example-gsp can be built and run based on the example of Ubuntu 20.04. Note that the following steps presume that the environment already fulfills the requirements for [dots-cpp](https://github.com/pnxs/dots-cpp#build-and-run).

Install build dependencies (includes the OpenGL SDK):

```sh
sudo apt-get update -yq && \
sudo apt-get install -yq \
    libgl-dev \
    xorg-dev
```

Clone repository:

```sh
git clone https://github.com/gerlachch/dots-example-gsp.git && cd dots-example-gsp
```

Build:

```sh
cmake -G Ninja -B ./build/ && cmake --build ./build/
```

Run:

```sh
./build/run.sh
```

# License

This project is currently licensed under the GPL-3.0.
