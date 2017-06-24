@echo off && setlocal

if "%1"=="" goto :bad
if "%2"=="" goto :bad
if "%3"=="" goto :bad
if "%4"=="" goto :bad


:start
	set usr=%1
	set pwd=%2
	set pge=%3
	set fil=%4
	
	:: works like this: GET /account/signin/ -> POST /account/signin/ -> auto-redir to downloads page -> POST downloads page
	
	:: GET initial csrf, dropped in the cookie, final 32 chars of the line containing that word
	:: [i] note: you can add the "-v" parameter to any cURL command to get a detailed/verbose output, useful to diagnose problems.
	echo getting initial csrf token from the sign-in page:
	curl -k -c cookies.txt --progress-bar -o nul https://bitbucket.org/account/signin/
	
	for /f "tokens=7 delims=	" %%A in ('type cookies.txt ^| find "csrf"') do set csrf=%%A
	
	:: and login using POST, to get the final session cookies, then redirect it to the right page
	echo signing in with the credentials provided:
	curl -k -c cookies.txt -b cookies.txt --progress-bar -o nul -d "username=%usr%&password=%pwd%&submit=&next=%pge%&csrfmiddlewaretoken=%csrf%" --referer "https://bitbucket.org/account/signin/" -L https://bitbucket.org/account/signin/
	
	for /f "tokens=7 delims=	" %%A in ('type cookies.txt ^| find "csrf"') do set csrf=%%A
	
	:: check that we have the session cookie, if not, something bad happened, don't spend time uploading.
	set session_cookie=
	for /f "tokens=6 delims=	" %%A in ('type cookies.txt ^| find "bb_session"') do set session_cookie=%%A
	if "%session_cookie%"=="" goto :notloggedin
	
	:: now that we're logged-in and at the right page, upload whatever you want to your repository...
	echo actual upload progress should appear right now as a progress bar, be patient:
	curl -k -c cookies.txt -b cookies.txt --progress-bar -o nul --referer "https://bitbucket.org/%pge%" -L --form csrfmiddlewaretoken=%csrf% --form token= --form files=@"%fil%" https://bitbucket.org/%pge%
	
	echo done? maybe. *crosses fingers* signing out, closing session!
	curl -k -c cookies.txt -b cookies.txt --progress-bar -o nul -L https://bitbucket.org/account/signout/
	
	goto :end
	
:notloggedin
	echo. 
	echo  [!] error: didn't get the session cookie, probably bad credentials or they changed stuff... upload canceled!
	goto :end

:bad
	echo  [!] error: missing arguments :(
	echo. 
	echo  syntax: upload-to-bitbucket.cmd ^<user^> ^<password^> ^<repo downloads page^> ^<local file to upload^>
	echo example: upload-to-bitbucket.cmd swyter secret1 /Swyter/tld-downloads/downloads myfile.zip
	echo. 
	
	pause

:end
