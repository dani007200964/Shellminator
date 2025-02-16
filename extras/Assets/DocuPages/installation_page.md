@page installation_page Installation

@tableofcontents

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Download.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" loop autoplay></lottie-player>
    </div>
    <div>
        <p>Before we get started with anything, we need to install this software package into our preferred development environment. We've gathered the most popular development environments and prepared a short guide for you to ensure a smooth start to this adventure.
        </p>
    </div>
</div>
\endhtmlonly

## Terminal Emulator Software

## Arduino Installation

Let's start with the installation guide for the Arduino environment, as we believe this is the simplest one of all. First, you will need the
[Arduino IDE](https://www.arduino.cc/en/software). This software is the heart of the Arduino ecosystem. Next, you'll need an Arduino board that has enough resources to meet your goals.

To help you find the most suitable board, we've created a table for you to easily choose one.

If you're just getting acquainted with this software, we recommend choosing a slightly more modern microcontroller, such as the Raspberry Pi Pico or the ESP32. We suggest this because these platforms have much more memory than, for example, an older Arduino Nano. This extra memory allows you to try out all the functions without needing to optimize right away. We're offering this friendly advice to help you avoid any additional distractions while you're learning.

Once you've installed the Arduino IDE, the next task is to install Shellminator. You can find detailed instructions
[here](https://docs.arduino.cc/software/ide-v2/tutorials/ide-v2-installing-a-library/).
Please make sure to always strive for using the latest version to ensure you have the most stable version on your computer.

## Arduino Installation Manually

There are, unfortunately, times when the Library Manager is not available. This can happen for various reasons, such as wanting to install the library offline, or working in a corporate environment where the paths used by the Arduino IDE for installation are blocked. But don't worry! The libraries can still be installed in these situations, please follow the instructions from [here](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/).

## STM32 Cube IDE

We absolutely love the STM32 family of microcontrollers—they're one of the most flexible and versatile options available today. And it's not just the hardware that's great; ST also provides a solid software ecosystem to support development.  

However, it's important to keep in mind that this flexibility comes at a cost. While the **Arduino IDE** is designed to be beginner-friendly, **Cube IDE** can feel a bit overwhelming at first. Not because it's a bad tool—far from it! The challenge is that it comes packed with a huge number of development tools, which can make it hard to know where to start.  

But don't worry! We’re here to help you set up a basic configuration.  

### Getting Started with STM32 Cube IDE
First, you'll need to **download and install Cube IDE** (unfortunately, this requires registration). During installation, **make sure to install all the necessary drivers** to avoid connection issues later.  

If you're new to STM32, we strongly recommend getting a [Nucleo board](https://www.st.com/en/evaluation-tools/nucleo-f446re.html). The **Nucleo** series was designed by ST specifically for learning and prototyping. It's affordable, widely available, and—most importantly—it comes with an **integrated debugger**. This built-in debugger makes the learning process **way easier** and saves you from needing additional hardware to start programming and debugging your STM32 projects.

Here's the English translation with a friendly and beginner-friendly tone:  

### Setting Up Your First STM32 Cube IDE Project

Once you’ve installed **STM32 Cube IDE**, the first step is to **open the IDE** and choose a workspace folder. This can be **anywhere on your computer**, just make sure it's a location you’ll remember. A single workspace can hold **multiple projects**, so you don’t need to create a new workspace for every project. For now, we’ll stick with the **default settings**. Click **Launch** to open the workspace.  

![Workspace](cube_workspace.png)

### Step 1: Sign in to Your ST Account
When the IDE opens, you'll need to **log in** to your ST account. You can do this by navigating to the **myST menu** in the top menu bar.  

![Login](cube_login.png)

### Step 2: Create a New STM32 Project
Once logged in, it’s time to create a **new project**. Navigate to:  
**File -> New -> STM32 Project**  

![New Project](cube_new_project.png)  

### Step 3: Select Your Nucleo Board or Microcontroller
Now, you need to **choose your board or microcontroller**:  
- If you’re using a **custom board or standalone microcontroller**, go to the **MCU/MPU Selector** tab.  
- If you’re using a **Nucleo** or another **ST development board**, use the **Board Selector** tab.  

For this guide, we’ll be using a **Nucleo-F746ZG**, so we’ll select it from the **Board Selector**.  

![Board Select](cube_board_select.png)

### Step 4: Configure Your Project
In the next window, you’ll set up the **project name** and **programming language**. You'll also choose:  
- The **binary format** (set this to **Executable**)  
- The **target project type** (set this to **STM32Cube**)  
- Since **Shellminator is a C++ framework**, make sure to select **C++** as the language.  

Once everything is set, click **Finish**.  

![Project Name](cube_project_name.png)

### Step 5: Use Default Peripheral Configuration
If Cube IDE asks whether it should **configure peripherals in default mode**, **choose "Yes"**. This will automatically set up the Nucleo board as intended by ST, so you won’t have to manually check the schematics to figure out pin assignments.  

![Default Settings](cube_periph_def_mode.png)

### Step 6: Open the Graphical Configurator
You might also see a prompt asking: **"Open Associated Perspective?"**  
Click **Yes**, as we’ll need to tweak some settings in the graphical configurator.  

![Open Configurator](cube_open_mx.png)

### Configuring the Microcontroller in CubeMX

The **Graphical Configurator** in CubeMX makes it easy to set up different microcontroller features. There are plenty of **YouTube** and **Udemy** tutorials covering this tool, and we highly recommend this series: [LINK].  

For now, we’ll **set up a UART interface** so we can communicate with our STM32. The rest of the features are up to you to explore!  

### Step 7: Configure the UART (USART) Interface

- In the **Connectivity** tab, look for a **USART peripheral**.  
- If you selected **default peripheral settings**, the configurator likely **already enabled one** for you.  
- This USART is **connected to the onboard debugger**, allowing direct serial communication with your PC.  

Since most STM32 chips have **multiple USART peripherals**, your Nucleo board **might be using USART2 or USART3** for this purpose. Check the board documentation to confirm. In our case, we’re using **USART3**.  

### Step 8: Configure DMA for UART Reception 

To avoid data loss when receiving data, we need to set up **DMA** (Direct Memory Access).  

1. Go to the **DMA Settings** tab.  
2. Click **Add** to create a new **DMA channel**.  
3. Set it to **USART_RX Request**.  
4. The **Priority** and **Stream** settings are flexible—adjust them based on your needs.  
5. Set **Direction** to **Peripheral to Memory**.  
6. In the **DMA Request Settings**, make sure **Mode** is set to **Circular**.  

DMA is important because it allows incoming data to be transferred **without CPU intervention**, preventing data loss and ensuring **non-blocking operation**—which is crucial for **Shellminator**.  

If your settings match the ones below, everything should work fine:  

![USART Settings](cube_usart_config.png)

### Step 9: Generate the Code  

1. **Save the project**. This will trigger a prompt asking if you’d like to **generate the code**. Click **Yes**.  
2. If the prompt doesn’t appear, you can manually generate the code by navigating to:  
   **Project -> Generate Code**  














### Step 10: Renaming main.c to main.cpp

At this point, we have successfully generated the **base code** for our microcontroller. However, we still need to make a few modifications.  

Even though we configured the project for **C++**, the code generator still creates a `main.c` file instead of `main.cpp`. To fix this, we need to **rename main.c to main.cpp**.  

#### Important Note for Future Code Generation

Since **main.c** contains both **auto-generated** and **manually written** code, it's crucial to avoid losing any changes. Before **regenerating** the code:  
1. Rename **main.cpp back to main.c**.  
2. Generate the code.  
3. Rename it back to **main.cpp**.  

This ensures that both your custom changes and the new auto-generated code are preserved in the correct file.  

### Step 11: Building the Project  
If everything was done correctly, the project should **compile successfully**. You can build it by selecting:  
- **Project -> Build All** from the menu  
- Or simply pressing **Ctrl + B**  

### Step 12: Adding Shellminator to the Project

First, [download the latest release of Shellminator](https://github.com/dani007200964/Shellminator/releases). Once downloaded:  
1. Place the **Shellminator folder** inside the **Drivers directory** of your project.  
2. If the new files don’t appear immediately in the file explorer, right-click the project and select **Refresh**.  

![Copy Shellminator](cube_shellminator_copy.png)

### Step 13: Adding Shellminator to the Include Paths
Next, we need to make sure the Shellminator source files are properly included in our project:  
1. Open the **Shellminator folder**.  
2. Right-click on the **src** folder and select **Add/Remove Include Path...**  
3. In the pop-up window, make sure **all targets** are selected (by default, this includes **Release** and **Debug**).  
4. Click **OK**.  

![Include Path](cube_src_folder_add.png)

### Step 14: Excluding Unnecessary Files
Shellminator comes with **extra files** that we don’t need for this project. These are located inside the **extras folder**, and if left included, they might cause **compilation errors**.  

To exclude them from the build process:  
1. Right-click the **extras** folder.  
2. Select **Resource Configurations -> Exclude from Build...**  
3. In the pop-up window, select **all targets** just like before.  
4. Click **OK**.

![Exclude Extras Folder](cube_extras_exclude.png)

### Step 15: Resolving the Missing Stream.h Error
If you try to compile now, you’ll likely get an error because the project is **missing Stream.h**. This happens because **Shellminator is designed to be compatible with the Arduino ecosystem**, and by default, it expects certain **Arduino libraries** —like **Stream** —to be available.  

Right now, our STM32 environment doesn’t include this functionality. But don’t worry! We’ve **prepared everything for you**, and you can easily add the missing libraries as part of a simple **software package installation**.  

Similar to how we set up Shellminator, you need to add the [SerialDriverForSTM32](https://github.com/dani007200964/SerialDriverForSTM32) package to your project. It’s best to download the [latest release](https://github.com/dani007200964/SerialDriverForSTM32/releases). Once downloaded, copy the software into your project’s `Drivers` folder.

In the `SerialDriverForSTM32` folder, you’ll find two important directories: `src` and `extras`. You need to add these to your project separately using **Right Click -> Add/Remove include path…**. In the pop-up window, make sure to check all your targets.

Now, if you try to build the project, it should compile without errors (though you might see some warnings—if you do, feel free to open an issue! Our goal is to get everything compiling warning-free too. 🙂)

### Step 16: Including the Necessary Libraries
In your `main.cpp`, you need to include the required headers:
```cpp
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Shellminator.hpp"
#include "Serial.hpp"
/* USER CODE END Includes */
```

### Step 17: Creating the Serial and Shellminator Objects
First, you need to create a `Serial` object. Since we configured `USART3`, we’ll use `huart3` in the constructor and name the object `usart3`.

Next, create a `Shellminator` object—let’s call it `shell` for now. The constructor will take the `usart3` object we just created.
```cpp
/* USER CODE BEGIN PV */
Serial usart3( &huart3 );
Shellminator shell( &usart3 );
/* USER CODE END PV */
```

### Step 18: Initializing the Serial Interface
You can initialize the `usart3` object using the `begin` method. Similar to the Arduino implementation, you need to specify the baud rate. We recommend **115200**, as it’s fast enough for modern terminals.

Once initialized, it's a good idea to clear the terminal interface using `shell.clear()`. Then, start the shell using `shell.begin("stm32")`. (The string can be anything you like.)
```cpp
/* USER CODE BEGIN 2 */
usart3.begin( 115200 );
shell.clear();
shell.begin( "stm32" );
/* USER CODE END 2 */
```

### Step 18: Running the Shell
Finally, inside the infinite loop, call `shell.update()` in every iteration.
```cpp
/* Infinite loop */
/* USER CODE BEGIN WHILE */
while (1)
{
    shell.update();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
}
```

### Running and Testing
If everything is set up correctly, your project should compile without issues. Once it compiles, you can run the code on your **Nucleo** board using the **Run** button in the **Run** tab.

To interact with your board, you’ll need a **terminal emulator**. If you’re not familiar with terminal emulators, don’t worry! We’ve detailed everything you need to know on the [Usage](usage_page.md) page. Since CubeIDE has a built-in terminal emulator, it’s worth checking out that section as well.

If you need more information about how the Shellminator API works, and what can it do, please check the [Examples](examples_page.md).

## Whole Code

```cpp
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Shellminator.hpp"
#include "Serial.hpp"
/* USER CODE END Includes */

...

/* USER CODE BEGIN PV */
Serial usart3( &huart3 );
Shellminator shell( &usart3 );
/* USER CODE END PV */

...

int main(void){

    ...

    /* USER CODE BEGIN 2 */
    usart3.begin( 115200 );
    shell.clear();
    shell.begin( "stm32" );
    /* USER CODE END 2 */

    ...

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        shell.update();
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}
```



















<div class="section_buttons">

| Previous          |                         Next |
|:------------------|-----------------------------:|
|                   | [Examples](@ref examples_page) |

</div>