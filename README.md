# cpplot
plotter for the cpp language.
work in progress.

Function rendering with customizable curve thickness is working.

![Red colored sine wave showing one period.](img/output1.png "Red colored sine wave showing one period.")

You can change the displayed section of cartesian space with one method call.

![Red colored sine wave showing two periods.](img/output2.png "Red colored sine wave showing two periods.")

It is also possible to change image resolution with one method call.

![Red colored sine wave showing two periods, but the image is shorter in height.](img/output3.png "Red colored sine wave showing two periods, but the image is shorter in height.")

Axis system is working. You can choose if the axis will be at a relative position with respect to the image or at a fixed position in R2 space. Various options for customization are available, including tick offset ,tick size, step size and more. If those arguments are not initialized by the user, the program chooses a suitable value on its own.

![Cyan quadratic function and Orange sine wave plotted over a cartesian plane with both x an y axis.](img/output4.png "Cyan quadratic function and Orange sine wave plotted over a cartesian plane with both x an y axis.")