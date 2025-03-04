@page faq_page Frequently Asked Questions
@tableofcontents

\htmlonly
<div style="display:flex; align-items: center;">
    <div style="width:150px; height:150px; margin-right: 20px;">
        <lottie-player src="Thinking-face.json" background="transparent" speed="1" style="width: 100%; height: 100%;" direction="1" playMode="normal" autoplay></lottie-player>
    </div>
    <div>
        <p>It's always great when questions arise in connection with such a project. Based on the experiences of recent years, we've gathered a few questions that many have asked, and we've tried to provide short, concise answers for each. If you need more information beyond these, feel free to ask on the <a href='https://github.com/dani007200964/Shellminator/discussions'>GitHub Discussions</a> platform.
        </p>
    </div>
</div>
\endhtmlonly

## Arduino Serial Monitor

Using the Arduino Serial Monitor, you may notice strange, almost alien characters. This happens because the Arduino Serial Monitor unfortunately does not support VT100 escape sequences, so it tries to display these special formatting commands as plain text with varying success. Since it cannot decode them, the formatting does not work properly.

If you prefer to stick with the Arduino Serial Monitor, you can disable formatting for basic functions using the following command but this is not a complete solution.
```cpp
shell.enableFormatting = false;
```

We recommend using a terminal emulator that supports decoding VT100 sequences. You can find assistance for this at the beginning of the [install](installation_page.md) section.


## Arduino BLE - ESP32

We ran into an interesting issue when using the ArduinoBLE library while working with the ESP32 core. The problem is that the ESP32 core already includes a version of ArduinoBLE that works for this platform, and the linker doesn’t like it when the same library is installed twice.

The fix? Simply remove the ArduinoBLE library from the library manager when using an ESP32, and you’ll be good to go!

## Windows Terminal UFT8 Support

If you're using the Windows terminal, you might notice that some characters don’t display correctly or are missing altogether. This happens because certain characters aren’t supported by default.  

Here’s a quick fix:  
1. Open the Run dialog ( `Win + R` ) and type **intl.cpl**, then press Enter. This will open the language settings.  
2. In the window that appears, go to the **Administrative** tab and click **Change system locale...**  
3. In the new window, check the box that says **Beta: Use Unicode UTF-8 for worldwide language support** at the bottom.  

After restarting your computer, everything should work as expected!  

For a more detailed explanation, check out [this helpful forum post](https://stackoverflow.com/questions/57131654/using-utf-8-encoding-chcp-65001-in-command-prompt-windows-powershell-window).

## Linux Terminal UFT8 Support

Some Linux distros, like Raspberry Pi OS, might not support UTF-8 characters in the terminal.
No worries, though—there’s an easy fix!
[This article walks you through the solution.](https://oorkan.medium.com/emojifying-your-linux-terminal-9a5c1e8f6b3c)