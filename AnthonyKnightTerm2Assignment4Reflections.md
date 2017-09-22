## **PID Controller Project**
### Writeup by Tony Knight - 2017/09/21

---

<img src="https://raw.githubusercontent.com/teeekay/CarND-PID-Control-Project/master/examples/videograb00003.png?raw=true"  width=1200>

<i><u>Figure 1: Snapshot from PID Controller Steering Video</u></i>

---

[Link to Youtube video](https://www.youtube.com/watch?v=BbUgvfqjlbA "PID Controller")



I found that I was able to use manual tuning to determine suitable PID parameters (Kp = 0.244, Ki = 0.0, and Kd = 8.91) that generally worked with the simulator on my computer until the car throttle went above 0.65.  

The results of tests completed with these PID parameter settings as the throttle rate was increased in subsequent runs of 12,000 iterations show, in Figure 2, how the cumulative error rate in each run increased almost in direct correlation to the throttle rate increase until an inflection point near the throttle setting of 0.65, beyond which the cumulative error metric increased dramatically.  This suggests that the more the throttle rate was increased beyond this point, the sooner the car left the track - increasing the number of extremely large CTE measurements.  

---


<img src="https://raw.githubusercontent.com/teeekay/CarND-PID-Control-Project/master/examples/ErrorvsThrottleRate.png?raw=true"  width=600>


<i><u>Figure 2: Automated testing results plotting the sum of the Square of CTE calculated over 12,000 iterations test runs were completed at different throttle rates</u></i>

---


The car was likely to crash within a couple of laps at throttle levels greater than 0.65, although when my computer was running smoothly, the car could complete multiple circuits of the track even at throttle rates as high as 0.75 (resulting in top speeds in mid 70's). The car might also crash at lower speeds if there was additional loading on my computer (like when I was trying to screengrab video of the simulation, or when my machine began indexing hard drives or running other background tasks - which I assume introduced extra latency between receiving offset measurements and applying the steer command).  This reinforced the need to use something like a real time OS or microcontroller to actually deal with moderating steering in a real car rather than through an OS like Microsoft Windows.

Although I was interested in using a system like twiddle to automatically tune the PID parameters, I found that manually tuning the parameters allowed me to observe how the PID process worked.  I initially compared the performance of PID parameters in a purely qualitative manner, as it was easy to see how poorly a parameter combination was working (when the car steered off the track or wobbled violently).  I initially worked on finding a value of Kp that caused the car to react quickly enough to curves or moving away from the center of the track, while adjusting Kd to moderate or dampen this behavior so that the car would not wobble from side to side.  Although I did investigate varying Ki it was left at zero as there was no observable steering bias away from the center of the road.  The settings and my observations/comments are included in the tables appended. 

I added in a quasi-quantitative metric which cumulatively summed the square of the CTE at each measurement.  I assumed that the time interval between measurements was relatively constant during and between runs.  I then compared how this metric increased/decreased when running the car around the track for a specific number of CTE measurements.  However, I noticed that this method was not a holistic measurement of the quality of the handling/ride of the car that the PID parameters produced:  

  1) I observed that as PID parameters were varied, the car sometimes went much further for the same number of CTE measurements even though the same throttle rate was applied.  I think this may have been because the steering was smoother.  A more choppy ride with more rapid changes in the direction of steering caused the velocity of the car to be reduced which resulted in the distance the car travelled being smaller for the same number of measurements.  However, the slower choppier ride might also produce a smaller CTE error than the faster smoother ride indicating, incorrectly in my opinion, that it was a better solution.

  2) I found that even when the throttle rate and velocity of the car were constant between test runs small differences in other aspects of the pose of the car (direction, offset from center, steering angle) when entering a curve could make a big difference in how the car reacted.  This suggested that to get good results with twiddle, would require that the car pass around the track multiple times so that the difference between runs would be minimized.  Otherwise the quantitative CTE measurements might be more reflective of the vehicle pose at the start of each turn as compared to the PID settings.  

  3) These findings indicate that much better results could be achieved (with need for less simulation) if exact car pose on the track (position, direction, initial steer, velocity) could be set for a variety of conditions on the simulator.  Quantitative measurement of PID performance could also be better achieved with access to the exact measurement time and pose of car on the track for each CTE measurement, in order to determine exactly how smooth the resulting path was, and which combination of PID parameters resulted in the car travelling the furthest distance around the track in the same time.

I also ran a couple of sets of tests when varying other parameters.  I tested how restricting the steering below the amounts allowed by the simulator affected performance.  I found that limiting the steering below less than +/- 14 degrees (55% of the allowed +/-25 degrees) began to significantly impact PID performance.  The difference above this amount was small, but it appeared to me that restricting the steering to +/- 15.5 degrees (62% of allowed amount) produced slightly better performance at higher throttle rates than letting the steering range between +- 25 degrees.

If I had additional time, I would have built twiddle functionality in, and would have tested if or how optimal P and D settings varied as the throttle rate and speed of the car was increased.  If such a correlation existed, a speed sensitive PID controller could then have been created.

----

### Appendices

----

#### PID Settings, Measurements and Observations used to tune the PID are appended below.

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
|	0.25	|	0	|	8	|	0.5	|	1	|	good, still touches edge of road on last curves and sways	|
|	0.27	|	0	|	10	|	0.5	|	1	|	too wobbly	|

---

### Try compensating for Bias - vary Ki

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

---

### Work on Smoothing out Steering - Vary Kd and also investigate reducing max steering Angle.  Also 
Increase speed to high levels to observe bad results



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

----

#### Graph and Table of Quantitative Test results as Kp varied

<img src="https://raw.githubusercontent.com/teeekay/CarND-PID-Control-Project/master/examples/ErrorvsKp.png?raw=true"  width=600>


<i><u>Figure 3: Automated testing results plotting the sum of the Square of CTE calculated over 6,000 iterations as Kp was varied</u></i>


|	Kp	|	Ki	|	Kd	|	max_steer	|	throttle	|	Error	|
|	----	|	----	|	----	|	----	|	----	|	----	|
|	0.236	|	0	|	10.9	|	0.475	|	0.5	|	2401.98	|
|	0.237	|	0	|	10.9	|	0.475	|	0.5	|	2502.65	|
|	0.238	|	0	|	10.9	|	0.475	|	0.5	|	2943.68	|
|	0.239	|	0	|	10.9	|	0.475	|	0.5	|	2432.07	|
|	0.24	|	0	|	10.9	|	0.475	|	0.5	|	2802.38	|
|	0.241	|	0	|	10.9	|	0.475	|	0.5	|	2525.54	|
|	0.242	|	0	|	10.9	|	0.475	|	0.5	|	2349.17	|
|	0.243	|	0	|	10.9	|	0.475	|	0.5	|	2591.55	|
|	0.244	|	0	|	10.9	|	0.475	|	0.5	|	2272.77	|
|	0.245	|	0	|	10.9	|	0.475	|	0.5	|	2665.34	|
|	0.246	|	0	|	10.9	|	0.475	|	0.5	|	2490.16	|
|	0.247	|	0	|	10.9	|	0.475	|	0.5	|	2316.7	|
|	0.248	|	0	|	10.9	|	0.475	|	0.5	|	2771.81	|
|	0.249	|	0	|	10.9	|	0.475	|	0.5	|	2459.65	|
|	0.25	|	0	|	10.9	|	0.475	|	0.5	|	2493.29	|


----

#### Graph and table of Quantitative Test results as Kd varied

<img src="https://raw.githubusercontent.com/teeekay/CarND-PID-Control-Project/master/examples/ErrorvsKd.png?raw=true"  width=600>


<i><u>Figure 4: Automated testing results plotting the sum of the Square of CTE calculated over 6,000 iterations as Kd was vaied - Error appears to increase as Kd increases beyond 11.0, but remain constant below this.  Chose to use maximum Kd of 10.9 where error level remains low </u></i>



|	Kp	|	Ki	|	Kd	|	max_steer	|	throttle	|	Smoothed error	|	Error	|
|	-	|	-	|	-	|	-	|	-	|	-	|	-	|
|	0.244	|	0	|	7.55	|	0.475	|	0.5	|		|	2126.9	|
|	0.244	|	0	|	7.6	|	0.475	|	0.5	|	2807.586667	|	2001.67	|
|	0.244	|	0	|	7.65	|	0.475	|	0.5	|	2651.814	|	4294.19	|
|	0.244	|	0	|	7.7	|	0.475	|	0.5	|	2819.866	|	2480.75	|
|	0.244	|	0	|	7.75	|	0.475	|	0.5	|	2870.278	|	2355.56	|
|	0.244	|	0	|	7.8	|	0.475	|	0.5	|	2454.192	|	2967.16	|
|	0.244	|	0	|	7.85	|	0.475	|	0.5	|	2380.222	|	2253.73	|
|	0.244	|	0	|	7.9	|	0.475	|	0.5	|	2644.658	|	2213.76	|
|	0.244	|	0	|	7.95	|	0.475	|	0.5	|	2500.346	|	2110.9	|
|	0.244	|	0	|	8	|	0.475	|	0.5	|	2554.396	|	3677.74	|
|	0.244	|	0	|	8.05	|	0.475	|	0.5	|	2601.476	|	2245.6	|
|	0.244	|	0	|	8.1	|	0.475	|	0.5	|	2632.608	|	2523.98	|
|	0.244	|	0	|	8.15	|	0.475	|	0.5	|	2326.42	|	2449.16	|
|	0.244	|	0	|	8.2	|	0.475	|	0.5	|	2639.836	|	2266.56	|
|	0.244	|	0	|	8.25	|	0.475	|	0.5	|	2607.254	|	2146.8	|
|	0.244	|	0	|	8.3	|	0.475	|	0.5	|	2595.422	|	3812.68	|
|	0.244	|	0	|	8.35	|	0.475	|	0.5	|	2627.976	|	2361.07	|
|	0.244	|	0	|	8.4	|	0.475	|	0.5	|	2696.49	|	2390	|
|	0.244	|	0	|	8.45	|	0.475	|	0.5	|	2483.03	|	2429.33	|
|	0.244	|	0	|	8.5	|	0.475	|	0.5	|	2452.04	|	2489.37	|
|	0.244	|	0	|	8.55	|	0.475	|	0.5	|	2472.882	|	2745.38	|
|	0.244	|	0	|	8.6	|	0.475	|	0.5	|	2649.846	|	2206.12	|
|	0.244	|	0	|	8.65	|	0.475	|	0.5	|	2557.622	|	2494.21	|
|	0.244	|	0	|	8.7	|	0.475	|	0.5	|	2586.212	|	3314.15	|
|	0.244	|	0	|	8.75	|	0.475	|	0.5	|	2806.112	|	2028.25	|
|	0.244	|	0	|	8.8	|	0.475	|	0.5	|	2708.192	|	2888.33	|
|	0.244	|	0	|	8.85	|	0.475	|	0.5	|	2455.62	|	3305.62	|
|	0.244	|	0	|	8.9	|	0.475	|	0.5	|	2572.61	|	2004.61	|
|	0.244	|	0	|	8.95	|	0.475	|	0.5	|	2544.946	|	2051.29	|
|	0.244	|	0	|	9	|	0.475	|	0.5	|	2423.254	|	2613.2	|
|	0.244	|	0	|	9.05	|	0.475	|	0.5	|	2479.398	|	2750.01	|
|	0.244	|	0	|	9.1	|	0.475	|	0.5	|	2539.636	|	2697.16	|
|	0.244	|	0	|	9.15	|	0.475	|	0.5	|	2561.416	|	2285.33	|
|	0.244	|	0	|	9.2	|	0.475	|	0.5	|	2490.326	|	2352.48	|
|	0.244	|	0	|	9.25	|	0.475	|	0.5	|	2549.048	|	2722.1	|
|	0.244	|	0	|	9.3	|	0.475	|	0.5	|	2650.926	|	2394.56	|
|	0.244	|	0	|	9.35	|	0.475	|	0.5	|	2682.714	|	2990.77	|
|	0.244	|	0	|	9.4	|	0.475	|	0.5	|	2642.396	|	2794.72	|
|	0.244	|	0	|	9.45	|	0.475	|	0.5	|	2642.372	|	2511.42	|
|	0.244	|	0	|	9.5	|	0.475	|	0.5	|	2564.922	|	2520.51	|
|	0.244	|	0	|	9.55	|	0.475	|	0.5	|	2475.012	|	2394.44	|
|	0.244	|	0	|	9.6	|	0.475	|	0.5	|	2593.954	|	2603.52	|
|	0.244	|	0	|	9.65	|	0.475	|	0.5	|	2571.514	|	2345.17	|
|	0.244	|	0	|	9.7	|	0.475	|	0.5	|	2616.798	|	3106.13	|
|	0.244	|	0	|	9.75	|	0.475	|	0.5	|	2592.416	|	2408.31	|
|	0.244	|	0	|	9.8	|	0.475	|	0.5	|	2609.456	|	2620.86	|
|	0.244	|	0	|	9.85	|	0.475	|	0.5	|	2528.864	|	2481.61	|
|	0.244	|	0	|	9.9	|	0.475	|	0.5	|	2540.224	|	2430.37	|
|	0.244	|	0	|	9.95	|	0.475	|	0.5	|	2495.688	|	2703.17	|
|	0.244	|	0	|	10	|	0.475	|	0.5	|	2463.242	|	2465.11	|
|	0.244	|	0	|	10.05	|	0.475	|	0.5	|	2544.966	|	2398.18	|
|	0.244	|	0	|	10.1	|	0.475	|	0.5	|	2550.768	|	2319.38	|
|	0.244	|	0	|	10.15	|	0.475	|	0.5	|	2557.632	|	2838.99	|
|	0.244	|	0	|	10.2	|	0.475	|	0.5	|	2629.49	|	2732.18	|
|	0.244	|	0	|	10.25	|	0.475	|	0.5	|	2725.06	|	2499.43	|
|	0.244	|	0	|	10.3	|	0.475	|	0.5	|	2725.338	|	2757.47	|
|	0.244	|	0	|	10.35	|	0.475	|	0.5	|	2782.198	|	2797.23	|
|	0.244	|	0	|	10.4	|	0.475	|	0.5	|	2749.9	|	2840.38	|
|	0.244	|	0	|	10.45	|	0.475	|	0.5	|	2652.602	|	3016.48	|
|	0.244	|	0	|	10.5	|	0.475	|	0.5	|	2549.526	|	2337.94	|
|	0.244	|	0	|	10.55	|	0.475	|	0.5	|	2490.82	|	2270.98	|
|	0.244	|	0	|	10.6	|	0.475	|	0.5	|	2617.324	|	2281.85	|
|	0.244	|	0	|	10.65	|	0.475	|	0.5	|	2659.364	|	2546.85	|
|	0.244	|	0	|	10.7	|	0.475	|	0.5	|	2615.044	|	3649	|
|	0.244	|	0	|	10.75	|	0.475	|	0.5	|	2661.736	|	2548.14	|
|	0.244	|	0	|	10.8	|	0.475	|	0.5	|	2662.428	|	2049.38	|
|	0.244	|	0	|	10.85	|	0.475	|	0.5	|	2445.816	|	2515.31	|
|	0.244	|	0	|	10.9	|	0.475	|	0.5	|	2496.902	|	2550.31	|
|	0.244	|	0	|	10.95	|	0.475	|	0.5	|	2657.794	|	2565.94	|
|	0.244	|	0	|	11	|	0.475	|	0.5	|	2642.332	|	2803.57	|
|	0.244	|	0	|	11.05	|	0.475	|	0.5	|	2708.72	|	2853.84	|
|	0.244	|	0	|	11.1	|	0.475	|	0.5	|	2675.854	|	2438	|
|	0.244	|	0	|	11.15	|	0.475	|	0.5	|	2654.72	|	2882.25	|
|	0.244	|	0	|	11.2	|	0.475	|	0.5	|	2645.416	|	2401.61	|
|	0.244	|	0	|	11.25	|	0.475	|	0.5	|	2658.508	|	2697.9	|
|	0.244	|	0	|	11.3	|	0.475	|	0.5	|	2595.852	|	2807.32	|
|	0.244	|	0	|	11.35	|	0.475	|	0.5	|	2627.608	|	2503.46	|
|	0.244	|	0	|	11.4	|	0.475	|	0.5	|	2664.98	|	2568.97	|
|	0.244	|	0	|	11.45	|	0.475	|	0.5	|	2629.308	|	2560.39	|
|	0.244	|	0	|	11.5	|	0.475	|	0.5	|	2617.974	|	2884.76	|
|	0.244	|	0	|	11.55	|	0.475	|	0.5	|	2599.096	|	2628.96	|
|	0.244	|	0	|	11.6	|	0.475	|	0.5	|	2617.294	|	2446.79	|
|	0.244	|	0	|	11.65	|	0.475	|	0.5	|	2585.692	|	2474.58	|
|	0.244	|	0	|	11.7	|	0.475	|	0.5	|	2541.754	|	2651.38	|
|	0.244	|	0	|	11.75	|	0.475	|	0.5	|	2547.014	|	2726.75	|
|	0.244	|	0	|	11.8	|	0.475	|	0.5	|	2738.688	|	2409.27	|
|	0.244	|	0	|	11.85	|	0.475	|	0.5	|	2762.934	|	2473.09	|
|	0.244	|	0	|	11.9	|	0.475	|	0.5	|	2727.048	|	3432.95	|
|	0.244	|	0	|	11.95	|	0.475	|	0.5	|	2782.094	|	2772.61	|
|	0.244	|	0	|	12	|	0.475	|	0.5	|	2815.342	|	2547.32	|
|	0.244	|	0	|	12.05	|	0.475	|	0.5	|	2599.476	|	2684.5	|
|	0.244	|	0	|	12.1	|	0.475	|	0.5	|	2583.098	|	2639.33	|
|	0.244	|	0	|	12.15	|	0.475	|	0.5	|	2712.32	|	2353.62	|
|	0.244	|	0	|	12.2	|	0.475	|	0.5	|	2690.384	|	2690.72	|
|	0.244	|	0	|	12.25	|	0.475	|	0.5	|	2766.134	|	3193.43	|
|	0.244	|	0	|	12.3	|	0.475	|	0.5	|	2818.504	|	2574.82	|
|	0.244	|	0	|	12.35	|	0.475	|	0.5	|	2854.25	|	3018.08	|
|	0.244	|	0	|	12.4	|	0.475	|	0.5	|	2793.39	|	2615.47	|
|	0.244	|	0	|	12.45	|	0.475	|	0.5	|	2831.244	|	2869.45	|
|	0.244	|	0	|	12.5	|	0.475	|	0.5	|	2744.994	|	2889.13	|
|	0.244	|	0	|	12.55	|	0.475	|	0.5	|	2780.352	|	2764.09	|
|	0.244	|	0	|	12.6	|	0.475	|	0.5	|	2717.304	|	2586.83	|
|	0.244	|	0	|	12.65	|	0.475	|	0.5	|	2700.894	|	2792.26	|
|	0.244	|	0	|	12.7	|	0.475	|	0.5	|	2674.256	|	2554.21	|
|	0.244	|	0	|	12.75	|	0.475	|	0.5	|	2725.354	|	2807.08	|
|	0.244	|	0	|	12.8	|	0.475	|	0.5	|	2584.318	|	2630.9	|
|	0.244	|	0	|	12.85	|	0.475	|	0.5	|	2680.204	|	2842.32	|
|	0.244	|	0	|	12.9	|	0.475	|	0.5	|	2703.446	|	2087.08	|
|	0.244	|	0	|	12.95	|	0.475	|	0.5	|	2737.804	|	3033.64	|
|	0.244	|	0	|	13	|	0.475	|	0.5	|	2751.364	|	2923.29	|
|	0.244	|	0	|	13.05	|	0.475	|	0.5	|	2865.668	|	2802.69	|
|	0.244	|	0	|	13.1	|	0.475	|	0.5	|	2818.206	|	2910.12	|
|	0.244	|	0	|	13.15	|	0.475	|	0.5	|	2742.934	|	2658.6	|
|	0.244	|	0	|	13.2	|	0.475	|	0.5	|	2692.55	|	2796.33	|
|	0.244	|	0	|	13.25	|	0.475	|	0.5	|	2662.716	|	2546.93	|
|	0.244	|	0	|	13.3	|	0.475	|	0.5	|	2627.224	|	2550.77	|
|	0.244	|	0	|	13.35	|	0.475	|	0.5	|	2554.956	|	2760.95	|
|	0.244	|	0	|	13.4	|	0.475	|	0.5	|	2627.628	|	2481.14	|
|	0.244	|	0	|	13.45	|	0.475	|	0.5	|	2720.702	|	2434.99	|
|	0.244	|	0	|	13.5	|	0.475	|	0.5	|	2719.998	|	2910.29	|
|	0.244	|	0	|	13.55	|	0.475	|	0.5	|	2766.612	|	3016.14	|
|	0.244	|	0	|	13.6	|	0.475	|	0.5	|	2854.274	|	2757.43	|
|	0.244	|	0	|	13.65	|	0.475	|	0.5	|	2798.778	|	2714.21	|
|	0.244	|	0	|	13.7	|	0.475	|	0.5	|	2781.778	|	2873.3	|
|	0.244	|	0	|	13.75	|	0.475	|	0.5	|	2757.11	|	2632.81	|
|	0.244	|	0	|	13.8	|	0.475	|	0.5	|	2874.566	|	2931.14	|
|	0.244	|	0	|	13.85	|	0.475	|	0.5	|	2832.232	|	2634.09	|
|	0.244	|	0	|	13.9	|	0.475	|	0.5	|	2861.162	|	3301.49	|
|	0.244	|	0	|	13.95	|	0.475	|	0.5	|	2786.53	|	2661.63	|
|	0.244	|	0	|	14	|	0.475	|	0.5	|	2828.814	|	2777.46	|
|	0.244	|	0	|	14.05	|	0.475	|	0.5	|	2728.732	|	2557.98	|
|	0.244	|	0	|	14.1	|	0.475	|	0.5	|	2726.196	|	2845.51	|
|	0.244	|	0	|	14.15	|	0.475	|	0.5	|	2787.204	|	2801.08	|
|	0.244	|	0	|	14.2	|	0.475	|	0.5	|	2844.94	|	2648.95	|
|	0.244	|	0	|	14.25	|	0.475	|	0.5	|	2790.858	|	3082.5	|
|	0.244	|	0	|	14.3	|	0.475	|	0.5	|	2781.696	|	2846.66	|
|	0.244	|	0	|	14.35	|	0.475	|	0.5	|	2772.476	|	2575.1	|
|	0.244	|	0	|	14.4	|	0.475	|	0.5	|	2704.102	|	2755.27	|
|	0.244	|	0	|	14.45	|	0.475	|	0.5	|	2833.032	|	2602.85	|
|	0.244	|	0	|	14.5	|	0.475	|	0.5	|	2961.624	|	2740.63	|
|	0.244	|	0	|	14.55	|	0.475	|	0.5	|	2929.398	|	3491.31	|
|	0.244	|	0	|	14.6	|	0.475	|	0.5	|	2902.202	|	3218.06	|
|	0.244	|	0	|	14.65	|	0.475	|	0.5	|	2977.232	|	2594.14	|
|	0.244	|	0	|	14.7	|	0.475	|	0.5	|	2777.566	|	2466.87	|
|	0.244	|	0	|	14.75	|	0.475	|	0.5	|	2627.912	|	3115.78	|
|	0.244	|	0	|	14.8	|	0.475	|	0.5	|	2711.76	|	2492.98	|
|	0.244	|	0	|	14.85	|	0.475	|	0.5	|	2790.264	|	2469.79	|
|	0.244	|	0	|	14.9	|	0.475	|	0.5	|	2817.466	|	3013.38	|
|	0.244	|	0	|	14.95	|	0.475	|	0.5	|	3041.52	|	2859.39	|
|	0.244	|	0	|	15	|	0.475	|	0.5	|		|	3251.79	|

----

#### Output of test runs with optimized PID settings



```sh
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
```

----


#### Output of test run where car remained on track until throttle rate exceeded 0.77


```sh
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
```