REM Build the date_time class DLL dependency which will be required by both
REM the consumer application and the other DLL project.
g++ -shared -DBUILDING_DATE_TIME_UTILITY=1 date_time_utility.cpp -o date_time_utility.dll
REM Build the user DLL project which will depend upon the date_time class, just
REM like the consumer application. We'll intentionally have a version mismatch
REM to show problems.
g++ -shared -DBUILDING_CHATROOM=1 -L. -l:date_time_utility.dll chatroom.cpp -o chatroom.dll
REM Build the application that we hope to demonstrate a crash owing to a
REM version-mismatch between a common DLL dependency of this application
REM and its user.dll dependency.
g++ -std=c++20 -L. -l:date_time_utility.dll -l:chatroom.dll crash.cpp -o crash
