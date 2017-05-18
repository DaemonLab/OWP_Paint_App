# OWP_Paint_App
First in a series of One Week Projects (OWP) which will focus on projects that can be made in approximately one week while still being useful. Users are expected to go through each commit and learn step by step from there.  
  
This is a windows only project. Cross platform gui projects will be covered in later OWPs. This is made with Win32 API in Code::Blocks IDE. The target audience is expected to know something about OOPs. And also should have atleast some idea of what are pointers and how they are handled.  
  
Learning the WIN32 API at first can be quite daunting and hence it is being covered first to show that it is really easy once you get hang of the terminologies. Moreover, once you get through this eemingly daunting API, learning other APIs (either on your own or via the OWPs) will become a piece of cake. Hope you enjoy this tutorial.  
  
## Running the code
- Download latest windows binary of Code::Blocks IDE from http://www.codeblocks.org/downloads and select the file that ends with "mingw-setup.exe" as it will contain the MinGW toolchains for windows which will be used for compiling and linking.  
- Clone this repo. And open the ".cbp" file which should open in the IDE.  
- Select Release mode in the toolbar and then build and run to see the code in action.  
  
  
Although the code is being provided, a learner is expected to run the code and go through the stages of how it was made. And then try him/her self to make some other software or an extension of the provided software. If you make something derived/inspired from this code, you can let us know and we can provide a link to your repository in a separate section of this readme.  
Very basic decription of each commit will be given in this readme. Learners are expected to use google to learn the in-depth intricasies. These tutorials are meant to promote learning-via-coding.  
  
## Steps
### Step 1
Make a new Win32 GUI project in Code::Blocks and select a frame-based layout. Code::Blocks will generate a lot of code for you which can seem daunting at first, but you will also see a lot of comments along with the code, read those and calm your nerves. The `WinMain` function is the actual main function for a Win32 Project. This is the entry point for your program. The specific signature of this function (A `WINAPI` modifier and the arguments) is required for the compiler to know which function is the main function.  
We generally also have a `WindowProcedure` function with its own specific signature. This function is used for all the callbacks, that is whenever an event occurs this function is called. An event can be the key press on a keyboard or moving of mouse or closing of window etc...  
All the data types that start with `H` are handles or pointers to some types. These are required as it saves memory and moreover you won't have to deal with freeing memory as windows will take care of it for you.  
First we have to register a class (or a model) for how the window (GUI) will look and then we can create a window (hwnd) using `CreateWindowEx` that create a window from the provided model and some other parameters. All the parameters are explained in the comments in the code.  
Then we will go into a while loop that will call `GetMessage` which return 0 only when the window has been destroyed/closed. In the loop we use the API to pass the event to the `WindowProcedure` that we defined. After the loop we exit with an exit-code. This is generally 0 to signify a normal exit and a non-zero value to signify exit on error. We will use the `wParam` set by windows API for this.  
At first, code of this tutorial can seem much, but believe me this will get exponentially easier in the later steps on the tutorial. Happy Coding! :)
