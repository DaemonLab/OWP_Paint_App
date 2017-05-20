# OWP_Paint_App
First in a series of One Week Projects (OWP) which will focus on projects that can be made in approximately one week while still being useful. Users are expected to go through each commit and learn step by step from there.  
  
This is a windows only project. Cross platform gui projects will be covered in later OWPs. This is made with Win32 API in Code::Blocks IDE. The target audience is expected to know something about OOPs. And also should have atleast some idea of what are pointers and how they are handled.  
  
Learning the WIN32 API at first can be quite daunting and hence it is being covered first to show that it is really easy once you get hang of the terminologies. Moreover, once you get through this seemingly daunting API, learning other APIs (either on your own or via the OWPs) will become a piece of cake. Hope you enjoy this tutorial.  
  
## Running the code
- Download latest windows binary of Code::Blocks IDE from http://www.codeblocks.org/downloads and select the file that ends with "mingw-setup.exe" as it will contain the MinGW toolchains for windows which will be used for compiling and linking.  
- Clone this repo. And open the ".cbp" file which should open in the IDE.  
- Select Release mode in the toolbar and then build and run to see the code in action.  
  
  
Although the code is being provided, a learner is expected to run the code and go through the stages of how it was made. And then try him/her self to make some other software or an extension of the provided software. If you make something derived/inspired from this code, you can let us know and we can provide a link to your repository in a separate section of this readme.  
Very basic decription of each commit will be given in this readme. Learners are expected to use google to learn the in-depth intricasies. These tutorials are meant to promote learning-via-coding.  
  
## Steps
### Step 1 -- Makin a window
Make a new Win32 GUI project in Code::Blocks and select a frame-based layout. Code::Blocks will generate a lot of code for you which can seem daunting at first, but you will also see a lot of comments along with the code, read those and calm your nerves. The `WinMain` function is the actual main function for a Win32 Project. This is the entry point for your program. The specific signature of this function (A `WINAPI` modifier and the arguments) is required for the compiler to know which function is the main function.  
We generally also have a `WindowProcedure` function with its own specific signature. This function is used for all the callbacks, that is whenever an event occurs this function is called. An event can be the key press on a keyboard or moving of mouse or closing of window etc...  
All the data types that start with `H` are handles or pointers to some types. These are required as it saves memory and moreover you won't have to deal with freeing memory as windows will take care of it for you.  
First we have to register a class (or a model) for how the window (GUI) will look and then we can create a window (hwnd) using `CreateWindowEx` that create a window from the provided model and some other parameters. All the parameters are explained in the comments in the code.  
Then we will go into a while loop that will call `GetMessage` which return 0 only when the window has been destroyed/closed. In the loop we use the API to pass the event to the `WindowProcedure` that we defined. After the loop we exit with an exit-code. This is generally 0 to signify a normal exit and a non-zero value to signify exit on error. We will use the `wParam` set by windows API for this.  
At first, code of this tutorial can seem much, but believe me this will get exponentially easier in the later steps on the tutorial. Happy Coding! :)  
  
### Step 2 -- Adding MenuBar and ToolBar
In this tutorial lets add menubar and toolbar to the app. It is always a good practice to keep your code organised into multiple files. Let's create utilities.h and utilities.cpp to hold our utility functions. The header file will hold constants and function declarations while the cpp file will hold the actual implementation of the functions. In utilites.h you can see some constants which are used to identify each button on toolbar and each menu separately. We will identify the file->new menu with same constant as that for the new button on toolbar. Goto utilities.cpp to see the implementation of the functions. Since `HWND` provides a handle to the original variable in memory, so we can add anything to our parent window if we have its handle.  
The commands in `addMenuBar` and `addToolBar` functions are properly explained in comments. You can google the API functions used to know more in detail about them.  
In `main.cpp` we have added a few more things in the `WindowProcedure`. Firstly, we are adding menubar and toolbar to our window once it is created. Actually, when a window is created, it issues a `WM_CREATE` signal once. This can be used to initialize stuff by the programmer. We have also added a case for `WM_COMMAND` which is issued on most events including keystrokes and clicks. We check the lower word (LOWORD), i.e., the last byte of the wParam as it contains the ID of the object which issued the command. We don't handle most of the menus now, but have placed placeholders for future. Also, note that the toolbar clicks will be caught here too since we are using same IDs for menus and toolbar buttons.  
  
If you find *undefined reference to addMenuBar* and similar errors then please right click on the project in CodeBlocks and add "utilities.h & utilities.cpp" to your project using "Add files" option.  
The commit for this step contains a minor error as I renamed the `addToolBar` (with a capital B) function in one of the files but not in others. You can easily fix this.

### Step 3 -- Allow drawing via a pencil tool
We will create 2 more files painter.h and painter.cpp to hold the code for actual painting on the screen. In win32 API, whenever the window has to be redraw it receives a `WM_PAINT` event which we will catch to draw on the window. But windows automatically clears the window before sending WM_PAINT, so we need to store the previous state of the window. For this, we will maintain a buffer, a copy of the actual window and draw on our copy. When we have to draw on the window we will simply copy our buffer onto the window, this way we will be safe from windows clearing our screen.  
Every window has a DC (Device Context) which can be thought of as a board for holding the canvas/paper. Every DC has a BITMAP which acts as the paper. We always issue commands on the DC but everything is draw on the BITMAP. In out painter object we create a DC and a BITMAP compatible with the actual window and then put the bitmap into the DC. We always draw on our local DC. When we have to update the actual window, we simply use `BitBlt` to copy our DC's content onto the screen's DC.  
Also, windows are not drawn regularly to avoid performance loss, hence we will have to force it to be drawn again and again. We do this by first invalidating the whole window and then asking windows to update it. Only the invalidated windows are updated by the API. For updating, the window is sent a WM_PAINT event where we can draw on the actaul window.  
Most of the Whys of the code have been explained in comments. But the actual format of every command and all other possibilities have not been explained. Search the MSDN documentation to learn those. Also, our pencil tool is quite buggy currently which we will improve later. Once you have a grasp over drawing graphics on windows, it will be easy to add other tools for drawing shapes which we will do in the next step of the tutorial.  

### Step 4
Coming soon...  
