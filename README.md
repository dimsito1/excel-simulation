# Excel Simulation Project

Welcome to the Excel Simulation project, a command-line interface application that simulates basic functionalities of an Excel spreadsheet. This application allows users to interact with and manipulate spreadsheet data through a variety of commands.
Table of Contents

![Excel Simulation](excel_simulation.png)

# Features

- **File Management**: Open existing text files as spreadsheets or create new ones on the fly.
- **Spreadsheet Interaction**: Navigate, edit, and manage data directly within the spreadsheet interface.
- **Formula Support**: Input and compute basic formulas, converting your spreadsheet into a powerful calculator.
- **Cell Editing**: Directly change the content of cells using easy-to-understand commands.
- **Data Visualization**: View your spreadsheet data in a clean, organized manner and observe the data types of each cell.
- **File Saving**: Safeguard your changes by saving your progress back into the text file.
- **Session Management**: Easily close your current spreadsheet, open another, or exit the program entirely when you're done.

# Requirements
- C++ compiler (e.g., g++)
- cmake

# Installation
    1. Clone the repository:
    ```shell
    git clone git@github.com:yourusername/excel-simulation.git
    ```

    2. Build the application:

    -if you are using a Linux distro (or Mac)-

    In the main path type:
    ```shell
    ./configure.sh
    ```
    then
    ```shell
    ./build.sh
    ```
    and finally
    ```shell
    ./run.sh
    ```

    -if you are using Windows
    ```shell
    mkdir build
    ```
    ```shell
    cd build
    ```
    ```shell
    cmake ..
    ```
    ```shell
    make
    ```
    and finally
    ```shell
    ./excel.exe
    ```    

# Getting Started

Once you've built the project, you can start interacting with the Excel Simulation application. To open an existing text file or create a new one if it does not exist, use the open <filename.txt> command:

```shell
>open example.txt
Text file opened successfully.
```

You can exit the program at any time using the exit command.

# Available Commands

Below is a list of commands available in the Excel Simulation project:
1. print

Displays the content of the currently opened spreadsheet.


```shell
>print

```
2. print spreadsheet

Shows the spreadsheet in a formatted manner, similar to how it would appear in Excel.

```shell
>print spreadsheet
```

3. print types

Displays the data types of the contents in each cell of the spreadsheet.

```shell
>print types
```

4. edit <row> <column> <new_value>

Edits the value of a cell in the spreadsheet using integer values for rows and columns.

```shell
>edit 1 2 150
```

5. save

Saves any changes made to the currently opened file.

```shell
>save
```

6. close

Closes the currently opened file, ensuring all changes are saved.

```shell
>close
```

7. exit

Exits the Excel Simulation program.

```shell
>exit
```

#How to Contribute

If you would like to contribute to the project or report issues, please feel free to open an issue or submit a pull request on our GitHub repository.
License


#License

This project is licensed under the MIT License.