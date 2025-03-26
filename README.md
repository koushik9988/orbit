# Orbit
Simple N-body gravity simulation

![ezgif com-video-to-gif-converter](https://github.com/koushik9988/orbit/assets/55924787/dfbc0d34-8350-4e6f-9aae-554ddd6a43b7)
![Figure_1](https://github.com/user-attachments/assets/f9a8b344-e025-40b5-801f-006177306892)
![Figure_2](https://github.com/user-attachments/assets/aca612fe-b7d2-44b9-97de-e68092854623)


### Installation
1. Clone the repository:
    ```bash
    git clone https://github.com/koushik9988/orbit.git
    ```

2. Navigate to the directory:
    ```bash
    cd orbit
    ```

3. Build the code using cmake:
    ```bash
    mkdir build && cd build
    ```
    ```bash
    cmake ..
    ```
    ```bash
    cmake --build .
    ```

### Running the Code
1. Configure the simulation parameters in the `input.ini` file.
2. Run the code:
The executble will be located in the build directory after building with cmake.
    ```bash
    ./orbit ../inputfiles/input.ini
    ```
