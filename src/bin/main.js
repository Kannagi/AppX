
import { testmodule } from 'module.js';

function Appx_main(argc,argv)
{
	Appx.print("hello world 5!");


	Appx.Window("Ma fenetre  test !");
	Appx.WindowIcon("favicon.png");

	Appx.print("init my web site"); //console.log

	//modify widget
	//Appx.WidgetInput(100,200);
	//Appx.WidgetButton(APPX_BUTTON_STYLE_0,"rect",100,100);

	Appx.start(100); // call update 100ms
}

global_Appx_main = Appx_main;

var count = 0;
function Appx_update(myapp,event)
{
	
	//Appx.print("count :",count);
/*
	if(count == 50)
		Appx.exit(); // exit program
*/
	count++;
	
}

global_Appx_update = Appx_update;