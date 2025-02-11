@page developer_zone_page Developer Zone

@tableofcontents

This section is for those fearless terminal ninjas who want to contribute to improving this system. First of all, we want to say a huge **thank you** for taking the time to make this software better and better!

To ensure that this shared adventure is a fun and productive challenge for everyone, we have a few simple guidelines:  

- **Be respectful and constructive** – Everyone starts somewhere, and we value all contributions, whether big or small. Keep discussions friendly and helpful.  
- **Clear and concise communication** – When reporting issues or submitting code, provide useful details that make it easy for others to understand and help.  
- **Follow the coding style** – Consistency is key. Try to adhere to existing conventions in the project to keep the codebase clean and maintainable.  
- **Write meaningful commit messages** – A good commit message helps others understand *why* a change was made, not just *what* changed.  
- **Be open to feedback** – Code reviews aren’t about criticism; they’re about making the project stronger. Let’s help each other improve!  
- **Keep it fun!** – At the end of the day, we’re all here to learn, build cool things, and collaborate. Let’s make this a positive space for everyone.  

### Necessary Tools

Now that we’ve got that covered, let’s dive in! To start developing, you’ll need a few extra tools:  
- [Visual Studio Code](https://code.visualstudio.com/) - We love this code editor
- [Anaconda](https://www.anaconda.com/download) - Helps with Python environments, you will need at least __version 4.12.0__
- [CMake](https://cmake.org/) - Helps building web examples, and desktop versions. You will need at least __version 3.24__
- [emscripten](https://emscripten.org/) - This builds the web examples. You will need at least __version 3.1.40__
- [Doxygen](https://www.doxygen.nl/) - This builds the whole html documentation. You will need at least __version 1.9.5__


Here’s the translated and slightly adapted version to keep it beginner-friendly and approachable:  

### Setting Up the Project Build & Documentation  

Translating and documenting this project is no small task, so we’ve automated a lot of it using the **build.py** script. To make sure it runs smoothly, you'll need a Python environment with all the required libraries installed.  

Good news: we’ve already gathered everything you need in a single list! All you have to do is open an **Anaconda Prompt** in the project’s root directory and run the following command:  

```sh
conda env create --file python_env.yml
```  

If everything goes well, this will set up all the necessary Python libraries for you.  

Next, in **Visual Studio Code’s terminal**, you need to activate the **shellminator** environment to start using it:  

```sh
conda activate shellminator
```  

Finally, if everything is set up correctly, you should be able to run the **build.py** script without issues:  

```sh
python build.py -t 'clean web'
```  

Here’s the translated and beginner-friendly version of your text:  

### How to Use build.py

To use `build.py`, you need to specify a **target** using the `-t` or `--target` option:  

- **clean** – Deletes previous builds to ensure a fresh start.  
- **web** – Compiles all examples using Emscripten, making them runnable in a web browser.  
- **examples** – Compiles all examples for the current platform (PC).  
- **test** – Meant for unit testing in the future, but it’s not functional yet!  

### Generating Example Code  

Before you can compile the examples, their source code needs to be generated for the target platform. This is handled automatically by the `exampleGenerator.py` script. Run the following command:  

```sh
python exampleGenerator.py
```  

### Compiling Examples for PC  

For example, if you want to compile all examples for PC, you can do it like this:  

```sh
python exampleGenerator.py
python build.py -t 'clean examples'
.\build\001_basic.exe
```  

### Useful Tips for Building  

- You **don’t** need to run `clean` before every build—doing so will slow things down significantly. Even though we compile on powerful machines, building all web examples still takes around **15 minutes**. Use `clean` for the **first** build since it’s necessary, but after that, you can skip it. Example:  
  ```sh
  python build.py -t 'web'
  ```  
- If you **don’t** want to compile everything, you can temporarily **comment out** unnecessary targets at the end of the `CMakeLists.txt` file using the `#` character.  
- If you **switch platforms**, you **must** run `clean`.  

### Generating Documentation  

1. Open the **Doxyfile** using the **Doxygen GUI** and generate the documentation by clicking **Run**.  
2. Then, compile the examples for the **WebAssembly** platform using:  
   ```sh
   python build.py -t 'clean web'
   ```  
3. Finally, locally host the documentation website using:  
   ```sh
   python ..\ShellminatorDocs\run_server.py
   ```