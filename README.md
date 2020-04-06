# fighting_corona
___
___
this project is a mechanical ventilator which was inspiered from  which will help the people who have a problems in breathing
first of all i want to **thank** [Darren Lewis](https://www.youtube.com/watch?v=jutBw_xIwTw&t=2s)  and  [Datatrician](https://www.youtube.com/watch?v=_6fWpdXvOYk&fbclid=IwAR2GE9O5BvPuQVyRz2XuBLzBiQLsaHiCa782UiRhk6GgAsO1CDTfBgqDVfA) because my project was inspired from them and actually i use [Darren Lewis](https://www.youtube.com/watch?v=jutBw_xIwTw&t=2s) designes and made some changes on it.
___
>### MACHINE FEATURES
* user can control of the volume of oxygen per breath
* user can control the speed of breathing
* user can control the delay between the inhale and exhale
* user can control the delay between 2 breaths
* user can moitor the pressure and the temperature on the breathing all time on the LCD
___
>### MAIN SCENARIO
1. welcome message 
```
  /*
   ###Fighting#####
   ####corona######
  */
```
2. init arm position to a certain position using min end stop
  
   ' #####please##### '
   ' #####wait!###### '
  
  
3. get target number of steps which refer to the volume of oxygen per breath you can use potentiometer to choose the value and press ok when you done
  
    ' volume of oxygen '
    ' ######10######## '
  

4. get delay between steps which refer to the speed of breathing you can use potentiometer to choose the value and press ok when you done
  
    ' #####speed###### '
    ' #######10####### '
  

5. get delay between inhale and exhale you can use potentiometer to choose the value and press ok when you done
  
   ' after inhale#### '
   ' wait###10####### '
  

6. get delay between 2 breaths you can use potentiometer to choose the value and press ok when you done
  
   ' after breath#### '
   ' wait###10####### '
  

7. now the doctor had set all prameters so the machine will mad 3 breaths to made the doctor sure from his setup
  
    ' #####wait!##### '
    ' ###for test#### '
  

8. now we should connect the device with patient and every single ms we had the values of the pressure and the temperature of his breath

   ' pressure=####10 '
   ' Temparature=#10 '

___
> #### NOTES
1. you can use any 8mm material to cut it and build the device
2. its easy to build i think it will take no longer than 30min with electronic connections
3. in this days i havn't any electronic simulator so i couldn't build a circuit diagram but i will attach some photos for every single module   conection
4. in ved_des folder you will find the .dxf files and the assembly, i made them with SOLIDWORKS2013 which is very old so i think every one had the same version or higher can edit them if he want 
5. you can use list_of_material.txt file to buy or know the components prices
6. now i'm working on controlling the device using feadback from patient

