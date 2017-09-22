## **PID Controller Project**

---

<img src="https://github.com/teeekay/CarND-Vehicle-Detection/blob/master/output_images/videograb00002.png?raw=true"  width=1000>

<i><u>Figure 1: Snapshot from Vehicle Detection Video</u></i>

---


### Writeup by Tony Knight - 2017/09/12 
Reflections


I found that I was able to use a 

|	Kp	|	Ki	|	Kd	|	throttle	|	Max_steer	|	Comments	|
|	----	|	----	|	----	|	----	|	----	|	----	|
|	0.08	|	0	|	0.8	|	0.3	|	1	|	got halfway round the lake, but wobbling was excessive from bridge onwards - increase D	|
|	0.08	|	0	|	1	|	0.3	|	1	|	this is a good balance, but does not steer sharp enough around 2nd and 3rd corner - increase p and D	|
|	0.1	|	0	|	1.2	|	0.3	|	1	|	made it around track, but was a bit wobbly - increase both p and d slightly (at max 35 mph)	|
|	0.13	|	0	|	1.4	|	0.3	|	1	|	made around track, too wobbly	|
|	0.13	|	0	|	1.8	|	0.3	|	1	|	pretty good, not quite enough steer response on sharper corners.  try boosting both p and d	|
|	0.15	|	0	|	2.5	|	0.3	|	1	|	better, but needs a tiny bit more	|
|	0.17	|	0	|	2.8	|	0.3	|	1	|	better, still too wobbly, and could be damped more	|
|	0.19	|	0	|	3.5	|	0.3	|	1	|	better, could stil be damped more, and tweak reaction	|
|	0.2	|	0	|	4	|	0.3	|	1	|	very good - try boosting damping	|
|	0.2	|	0	|	6	|	0.3	|	1	|	turns a bit sharp, but enables fast reaction - try boosting velocity now to 0.5	|
|	0.2	|	0	|	6	|	0.5	|	1	|	takes car to 55 now, and car does not react quite quickly enough on last set of curves, and is wobbly - try	|
|	0.25	|	0	|	8	|	0.5	|	1	|	good, still toches edge of road on last curves and sways	|
|	0.27	|	0	|	10	|	0.5	|	1	|	too wobbly	|

### Try Bias - Ki

|	Kp	|	Ki	|	Kd	|	throttle	|	Max_steer	|	Comments	|
|	----	|	----	|	----	|	----	|	----	|	----	|
|	0.23	|	0.1	|	10	|	0.5	|	1	|	goes in circles!	|
|	0.23	|	0.001	|	10	|	0.5	|	1	|	wobbly touches edges	|
|	0.23	|	0.005	|	10	|	0.5	|	1	|	too much, causes sharp wobbles after 4th curve	|
|	0.23	|	0.003	|	10	|	0.5	|	1	|	too much, causes sharp wobbles after 4th curve	|
|	0.26	|	0.003	|	10	|	0.5	|	1	|	too much, causes sharp wobbles after 6th curve	|
|	0.26	|	0.002	|	10	|	0.5	|	1	|	crashes	|
|	0.26	|	0.002	|	6	|	0.5	|	1	|	crashes	|
|	0.26	|	0.0015	|	16	|	0.5	|	1	|	around track pretty well on very edge of paint at corner 5/6	|
|	0.26	|	0.0015	|	20	|	0.5	|	1	|	hits paint at corner 4	|
|	0.27	|	0.0015	|	20	|	0.5	|	1	|	hits paint at corner 4	|
|	0.27	|	0.00125	|	20	|	0.5	|	1	|	hits paint at 4 and 5	|
|	0.27	|	0.0011	|	20	|	0.5	|	1	|	> error 0.9-0.85	|
|	0.27	|	0.0011	|	20	|	0.6	|	1	|		|
|	0.27	|	0.008	|	20	|	0.6	|	1	|	crash on first corner - swerving violently	|

### Work on Smoothing out Steering Kd and also investigate reducing max steering Angle
Increasing speed to high levels to observe bad results

|	Kp	|	Ki	|	Kd	|	throttle	|	Max_steer	|	Comments	|
|	----	|	----	|	----	|	----	|	----	|	----	|
|	0.27	|	0	|	20	|	0.6	|	1	|	makes it round, touching paint at 5/6 a bit too much swerve - top speed 59 at corner 1	|
|	0.27	|	0	|	20	|	0.7	|	1	|	makes it roundto corner 6 nicely, but too much swerving therecrashing  - top speed 65 at corner 2	|
|	0.27	|	0	|	25	|	0.7	|	1	|	better, generally makes it around, but can crash when weaving pattern started	|
|	0.26	|	0	|	25	|	0.7	|	0.9	|	nice - but wobbles get out of control	|
|	0.26	|	0	|	25	|	0.7	|	0.8	|	pretty good - still can crash	|
|	0.26	|	0	|	25	|	0.7	|	0.7	|	wobbliness accentuated	|
|	0.25	|	0	|	25	|	0.7	|	0.7	|	crashes on 4- 5 wobbles	|
|	0.25	|	0	|	15	|	0.8	|	0.5	|	crashes on wobbles	|
|	0.25	|	0	|	10	|	0.8	|	0.5	|	made it round 1 lap top speed 80mph crashed on wobbles	|
|	0.25	|	0	|	8	|	0.8	|	0.5	|	super wobble	|
|	0.25	|	0	|	12	|	0.8	|	0.5	|	super wobble	|
|	0.25	|	0	|	9	|	0.8	|	0.5	|	super wobble	|
|	0.25	|	0	|	11	|	0.8	|	0.5	|	pretty good - big smash on curve 5	|
|	0.24	|	0	|	11	|	0.8	|	0.4	|	too wobbly crash curve 1	|
|	0.24	|	0	|	11	|	0.8	|	0.4	|	wobbled between 4-5 crash at 5	|
|	0.24	|	0	|	11	|	0.8	|	0.35	|		|
|	0.245	|	0	|	12	|	0.8	|	0.35	|		|
|	0.245	|	0	|	10.5	|	0.8	|	0.35	|	smash at curve 2	|
|	0.245	|	0	|	10.5	|	0.8	|	0.45	|		|
|	0.2475	|	0	|	10.5	|	0.8	|	0.45	|	wobbled on bridge - crash	|
|	0.2425	|	0	|	10.5	|	0.8	|	0.45	|	almost around track	|
|	0.24125	|	0	|	10.5	|	0.8	|	0.45	|	crash on curve 5	|
|	0.24375	|	0	|	10.5	|	0.8	|	0.45	|	crash on curve 5	|
|	0.243	|	0	|	10.5	|	0.8	|	0.45	|	crash on curve 5	|
|	0.242	|	0	|	10.5	|	0.8	|	0.45	|	crash on curve 5	|
|	0.2428	|	0	|	10.5	|	0.8	|	0.45	|	too wobbly crash on bridge	|
|	0.242	|	0	|	10.5	|	0.8	|	0.45	|		|
|	0.242	|	0	|	10.7	|	0.8	|	0.45	|	slightly better	|
|	0.242	|	0	|	10.8	|	0.8	|	0.45	|	better moderation, but bad result	|
|	0.242	|	0	|	10.8	|	0.75	|	0.45	|	better moderation, but bad result	|
|	0.242	|	0	|	10.75	|	0.75	|	0.45	|	better moderation, but bad result	|
|	0.242	|	0	|	10.9	|	0.75	|	0.45	|		|





Using 0.3 as throttle on this run.

Listening to port 4567Connected!!!
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
Connected!!!om -28.8919 to  -0.75. 113, D: -28.1217 I: -0
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
 Iteration: 8000, Err: 1506.23 P: -0.0277672, D: -0 I: -0 I: -00

Boosting throttle to 0.31 after 8000 iterations
 Iteration: 16000, Err: 2917.61 P: -0.0746396, D: 0.147015 I: -00

Boosting throttle to 0.32 after 16000 iterations
 Iteration: 24000, Err: 4367.88 P: -0.057462, D: 0.089991 I: -000

Boosting throttle to 0.33 after 24000 iterations
 teration: 32000, Err: 6162.63 P: -0.314174, D: -0 I: -069 I: -00

Boosting throttle to 0.34 after 32000 iterations
 Iteration: 40000, Err: 8308.36 P: -0.007564, D: -0.03564 I: -000

Boosting throttle to 0.35 after 40000 iterations
 Iteration: 48000, Err: 9853.59 P: -0.059048, D: 0.065934 I: -000

Boosting throttle to 0.36 after 48000 iterations
 ^Ceration: 49412, Err: 10130.7 P: -0.0180316, D: -0 I: -05 I: -0
teeekaay@TK-NZXT:/mnt/c/Users/tknight/Source/Repos/CarND/CarND-PID-Control-Project/build$ ./pid
To specify the throttle value to use at run time call

./pid throttle_value

like this:
./pid 0.5
throttle_value should be a float between 0.1 and 0.999.
Otherwise, 0.3 will be used by default.
Using 0.3 as throttle on this run.

Listening to port 4567Connected!!!
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
Connected!!!om 378.717 to  0.75.  10.0947, D_err: 368.623 I_err: 0
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
 ^Ceration: 6804, Err: 1384.1 P_err: -0.100357, D_err: -0 I_err: -0err: -000
teeekaay@TK-NZXT:/mnt/c/Users/tknight/Source/Repos/CarND/CarND-PID-Control-Project/build$ ./pid 0.55
The initial throttle argument supplied is 0.55
Using 0.55 as throttle on this run.

Listening to port 4567Connected!!!
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
Connected!!!om 613.069 to  0.75.  16.3414, D_err: 596.728 I_err: 0
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
 teration: 8000, Err: 3226.97 P_err: -0.406724, D_err: -0 I_err: -0err: -000

Boosting throttle to 0.56 after 8000 iterations
 ^Ceration: 9156, Err: 4201.3 P_err: -0.0245464, D_err: -0.236115 I_err: -00
teeekaay@TK-NZXT:/mnt/c/Users/tknight/Source/Repos/CarND/CarND-PID-Control-Project/build$ ./pid 0.66
The initial throttle argument supplied is 0.66
Using 0.66 as throttle on this run.

Listening to port 4567Connected!!!
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
Connected!!!om 891.704 to  0.75.  23.7684, D_err: 867.936 I_err: 0
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
 Iteration: 8000, Err: 5529.67 P_err: -0.0503616, D_err: -0 I_err: -0rr: -00

Boosting throttle to 0.67 after 8000 iterations
 Iteration: 16000, Err: 10539.2 P_err: -0.206644, D_err: -0 I_err: -0rr: -000

Boosting throttle to 0.68 after 16000 iterations
 Iteration: 24000, Err: 17686.8 P_err: -0.12161, D_err: -0 I_err: -0rr: -0000

Boosting throttle to 0.69 after 24000 iterations
 Iteration: 32000, Err: 25548.1 P_err: -0.038674, D_err: -0 I_err: -0rr: -000

Boosting throttle to 0.7 after 32000 iterations
 Iteration: 40000, Err: 30640.5 P_err: -0.169556, D_err: 0.386694 I_err: -000

Boosting throttle to 0.71 after 40000 iterations
 teration: 48000, Err: 37263.6 P_err: -0.741418, D_err: -0 I_err: -0_err: -00

Boosting throttle to 0.72 after 48000 iterations
 Iteration: 56000, Err: 42021.8 P_err: -0.324618, D_err: -0 I_err: -0err: -00

Boosting throttle to 0.73 after 56000 iterations
 Iteration: 64000, Err: 47810.4 P_err: -0.0512156, D_err: 0.130086 I_err: -00

Boosting throttle to 0.74 after 64000 iterations
 Iteration: 72000, Err: 55212.4 P_err: -0.179486, D_err: -0 I_err: -0rr: -000

Boosting throttle to 0.75 after 72000 iterations
 Iteration: 80000, Err: 75318.4 P_err: 0.417118, D_err: -0.377784 I_err: -000

Boosting throttle to 0.76 after 80000 iterations
 Iteration: 88000, Err: 81962.7 P_err: -0.0226432, D_err: -0.250371 I_err: -0

Boosting throttle to 0.77 after 88000 iterations
 Iteration: 96000, Err: 87303 P_err: 0.0269132, D_err: -0.33858 I_err: -00-00

Boosting throttle to 0.78 after 96000 iterations
^Cx Steer from 4.65942 to  0.75. rr: 4.81178, D_err: -0.152361 I_err: -000000
teeekaay@TK-NZXT:/mnt/c/Users/tknight/Source/Repos/CarND/CarND-PID-Control-Project/build$ ./pid 0.78
The initial throttle argument supplied is 0.78
Using 0.78 as throttle on this run.

Listening to port 4567Connected!!!
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
Connected!!!om 93.0458 to  0.75.  2.48014, D_err: 90.5657 I_err: 0
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
^Cx Steer from 2.72963 to  0.75. rr: 2.72963, D_err: -0 I_err: -0r: -0000000
teeekaay@TK-NZXT:/mnt/c/Users/tknight/Source/Repos/CarND/CarND-PID-Control-Project/build$ ./pid 0.77
The initial throttle argument supplied is 0.77
Using 0.77 as throttle on this run.

Listening to port 4567Connected!!!
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
Connected!!!om 202.33 to  0.75. : 5.39311, D_err: 196.937 I_err: 0
(Kp,Ki,Kd) set to (0.244, 0, 8.91).

Limiting maximum steer to (-0.75,0.75).
 Iteration: 8000, Err: 4464.89 P_err: -0.0597312, D_err: -0 I_err: -0rr: -00

Boosting throttle to 0.78 after 8000 iterations
 Iteration: 16000, Err: 9973.79 P_err: -0.0763964, D_err: 0.135432 I_err: -00

Boosting throttle to 0.79 after 16000 iterations
^Cx Steer from 3.5634 to  0.75. _err: 3.5634, D_err: -0 I_err: -0rr: -000-000