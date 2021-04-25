# Minishell

<pre>

------------------------------------------------------------------------------

Lifetime of a Shell:

A shell does three main things in its lifetime:

	Initialize:
	In this step, a typical shell would read and execute its configuration files.
	These change aspects of the shell’s behavior.

	Interpret:
	Next, the shell reads commands from stdin (which could be interactive, or a file)
	and executes them.

	Terminate: After its commands are executed, the shell executes any shutdown
	commands, frees up any memory, and terminates.


The program needs to loop in order to work. And in the most basic form, the loop
will do 3 things:

	Read:
	Read the command from standard input.

	Parse:
	Separate the command string into a program and arguments.

	Execute:
	Run the parsed command.

------------------------------------------------------------------------------

get_next_line (fd, buffer(**line)).
									Inside of gnl you have a buffer_size.
									You can change it on get_next_line.h

stdin:  Same as fd = 0.		<--- This is what the keyboard writes.
stdout: Same as fd = 1. 	<--- This is what the shell terminal shows.
stderr: Same as fd = 2. 	<--- This is what the shell terminal shows.

------------------------------------------------------------------------------

We don't know which process starts first using fork() that's why we use
wait on the parent so the child goes first and the parent waits until the child
finishes.

------------------------------------------------------------------------------

Commands:

	builtin:

		absolute_path (NO Options, NO Args) // Changes prompt from "minishell$ " to absolute path.
		testing		  (NO Options, NO Args) // Changes subject commands for real builtin commands.
		history		  (NO Options, NO Args) // List of the history of things you did.

		echo		  (-n Option,  YES Args) // -n  Do not output a trailing newline.
		pwd			  (NO Options, YES Args)
		export		  (NO Options, YES Args)
		unset		  (NO Options, YES Args)
		env			  (NO Options,  NO Args)
		exit		  (NO Options, YES Args)
		cd			  (relative / abs path)

------------------------------------------------------------------------------

$? - Shows the exit status of the last command.

	Example:

		Type ls on the console. It worked.
		Type now $?, the previous exited status was succesful (0).

		Type wqweqwe. It gave an error.
		Type now $?, the previous exited status gave an error (127).

		Value 127 is returned by your shell /bin/bash when any given
		command within your bash script or on bash command line is not
		found in any of the paths defined by PATH system environment variable.
		The solution is to make sure that the command your are using can be
		found within your $PATH.

		https://tldp.org/LDP/abs/html/exitcodes.html

------------------------------------------------------------------------------

How our program works with examples:

	These examples don't mention error handlings. If you're following the steps you'll be
	able to identify them and do your error handlings easily.


	- Parsing to Command Tables:

	Global Variable:
		Our global variable (mini_sh) is a struct that contains these essential variables
		for parsing:
		- char *line = The line you wrote on your prompt and got from get_next_line().
		- char **cmd_tables = Where you're going to store your command tables.


	Example in action:	 > echo "hello" how are "you

	Step 1:
		We do "get_next_line(0, &line)" to take what the user writes in the prompt.
		It's stored in -> mini_sh.line.

							mini_sh.line = echo "hello" how are "you

		We check if it's NULL (or if the user pressed entered without printing anything).
		If the user did, we do "continue;" until he writes something.

	Step 2:
		Check if the mini_sh.line has wrong quotations. check_complete_quotation().
		It can't have open " or open '.
		" " = Valid!
		'"' = Valid!
		"'  = Invalid!

							echo "hello" how are "you

		Has a open ". Therefor it's	incorrect.
		We return a (0).
		We start all over again from Step 1.
		We now put a new line on the STDIN.



	Example in action:	 > echo "hello ; bye" how are "you" ; echo "I'm fine!" thank you ; ls -a ; env

		Step 1:
			mini_sh.line = echo "hello ; bye" how are "you" ; echo "I'm fine!" thank you ; ls -a ; env

		Step 2:
			It is valid! Quotations are not open!

		Step 3:
			We now check if mini_sh.line has ; so we can divide it by command tables.
			To do that we check the entire string for ";" that are NOT inside " " or ' '.
			After doing that, we take the locations of the ";" and how many there are.

				 echo "hello ; bye" how are "you" ; echo "I'm fine!" thank you ; ls -a ; env
												  .							   .       .
												 /|\						  /|\     /|\
												  |							   |	   |
												  38						   61	   69

			We have 3 ";" at locations 38, 61 and 69.
			We now have to divide the first line from 0 to 38, the second from 39 to 61, the
			third from 62 to 69 and the last one from 70 to the end.

			We now have:

			line 1:		echo "hello ; bye" how are "you" ;
			line 2:		echo "I'm fine!" thank you ;
			line 3:		ls -a ;
			line 4:		env
			line 5:		NULL

			Always add a last line that is NULL why is that?

			If you do "while (cmd_tables[i] != NULL)", it will scroll all the way to the last
			line. If you don't put line 5 as NULL, that command won't work.

		Step 4:
			We now check if we have an empty last line. Where it says line 4: env , there are
			cases it's empty "'\0", if it's empty, we set it to NULL.
			After doing that we remove all ";" from the last character.

			line 1:		echo "hello ; bye" how are "you"
			line 2:		echo "I'm fine!" thank you
			line 3:		ls -a
			line 4:		env
			line 5:		NULL

		Step 5:
			We now put the lines we made into the global variable we called cmd_tables.
			It's our global variable called mini_sh.cmd_tables.

			mini_sh.cmd_tables[0]:	echo "hello ; bye" how are "you"
			mini_sh.cmd_tables[1]:	echo "I'm fine!" thank you
			mini_sh.cmd_tables[2]:	ls -a
			mini_sh.cmd_tables[3]:	env
			mini_sh.cmd_tables[4]:	NULL









line = echo "hello" olah" " ; echo "adeys" olah" " | grep la

command table[0] = echo "hello" olah" ";
command table[1] = echo "adeys" olah" " | grep la;

args[0] = echo
args[1] = "hello"
args[2] = olah
args[3] = " "
args[4] = '\0'

execve()

------------------------------------------------------------------------------

Signals:

	You can type "stty -a" on your console to see what all terminal line settings are.

	intr = ^C;				SIGINT
	eof = ^D;
	quit = ^\;				SIGQUIT


	intr CHAR
		CHAR will send an interrupt signal
	eof CHAR
		CHAR will send an end of file (terminate the input)
	quit CHAR
		CHAR will send a quit signal

	https://pubs.opengroup.org/onlinepubs/7908799/xsh/signal.h.html

------------------------------------------------------------------------------

Errors:

	Errors are checked with $?

		Quotation Marks Incomplete. (No real error. Just minishell requirement)
		bash: syntax error near unexpected token ';' (2: command not found)



------------------------------------------------------------------------------


Updates:
		Added reading from executable files. ./a.out or /srcs/exec/exectest etc etc...
		Added History to the prompt. (No repeats, no blank spaces).

To Update:
		Change mini_sh to g_minish (Norminette).

To Take in Consideration:
		echo ola\n --> This removes \


gcc -fsanitize=address -o minishell shell.c ./srcs/parsing/* ./srcs/allocate/* ./srcs/utils/* ./srcs/builtin/*c ./srcs/exec/*.c ./get_next_line/*.c libft/libft.a


</pre>
