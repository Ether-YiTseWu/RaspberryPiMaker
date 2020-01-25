<?php 
echo "Do you want to light up the LED ? ", $_POST["LED_Light"], "<br>";

if ($_POST["LED_Light"] == "yes")
{
   exec ("sudo python LED_YES.py");
   echo "Light ON";
}
else
{
   exec ("sudo python LED_NO.py");
   echo "Light OFF";
}

?>