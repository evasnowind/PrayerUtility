package com.prayer.testdemo;

import android.view.MotionEvent;
import android.view.View;

public class ViewUtils {

	/**
     * 判断点击事件ev是否发生在view视图内
     * @param view
     * @param ev
     * @return
     */
    public boolean inRangeOfView(View view, MotionEvent ev){
    	if(null != view && null != ev){
    		int[] location = new int[2];
    		view.getLocationOnScreen(location);
    		int x = location[0];
    		int y = location[1];
    		if(ev.getX() < x || ev.getX() > (x + view.getWidth()) 
    				|| ev.getY() < y || ev.getY() > (y + view.getHeight())){
    			return false;
    		}
    		return true;
    	} else {
    		return false;
    	}
    }
}
