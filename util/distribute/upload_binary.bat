@echo off
REM Remove the REM below and edit the settings if you want to upload the releases somewhere.
REM ncftpput -u user -p password ftp.yourftp.net /path_on_ftp_server/Q2DOS %1


REM Requires a directory in root q2dos dir "upload_to_bitbucket" because it
REM requires a libcurl with SSL which we can't distribute because
REM Windows 98 doesn't support it

REM cd upload_to_bitbucket
REM CALL upload_to_bitbucket.cmd user password neozeed/q2dos/downloads %1

REM cd ..
