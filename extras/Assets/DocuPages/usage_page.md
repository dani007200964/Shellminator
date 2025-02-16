@page usage_page Usage

@tableofcontents

### Useful Tips for Getting Started  

Before you begin, you’ll need a **terminal emulator** on your computer. Here are our recommendations:  

- **Windows:** We suggest using **PuTTY** because it’s free and has worked with all features so far. You can use it for both **TCP** and **serial port** connections.  
- **macOS & Linux:** The built-in terminal emulators usually come with the necessary tools pre-installed.  
  - To list available serial ports, use: `ls /dev/tty.*`
  - To connect via serial port, use: `screen PORT PORT_SPEED` - Example: `screen /dev/ttyS0 115200`
  - To close screen session use: `CTRL-A` + `:quit`
  - To connect via **TCP**, use: `telnet IP PORT` - Example: `telnet 192.168.0.160 23`
  - To close **telnet session** use: `CTRL-]` + `:quit`
- **Linux:** If these commands don’t work, the required packages might not be installed. You can fix this by running:
  ```sh
  sudo apt-get install telnet screen
  ```  
- **macOS:** If these commands don’t work, the required packages might not be installed. You need homebrew for newer macOS variants to install telnet.
  ```sh
  sudo apt-get install telnet screen
  ```  

### Missing Characters or Emojis on Linux?  

On Linux, you might run into a weird issue where **some characters don’t display correctly** because they’re missing from the UTF-8 table. This can make things like emojis look strange.  

If you're using **Ubuntu**, this usually isn’t a problem. However, on systems like **Raspberry Pi OS**, emojis are often missing by default.  

If you need to install them, [this guide](https://oorkan.medium.com/emojifying-your-linux-terminal-9a5c1e8f6b3c) can help you get everything set up! 😊  
