package com.android.bankabc.widget.gridview;

import com.rytong.emp.tool.Utils;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.text.TextUtils;
import android.util.AttributeSet;
import android.view.View;
import android.widget.GridView;

/**
 * 带网格线的GridView
 */
public class LineGridView extends GridView {

	private boolean isDrawBorder = false;
	private boolean isDrawBorderAll = false;
	private String mLineColor;
	private float mLineWidth;
	private boolean isDrawReminderBorder = false;
	
	public LineGridView(Context context) {
		super(context);
	}

	public LineGridView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}

	public LineGridView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}
	
	public void init(String lineColor, float lineWidth, boolean isDraw, boolean isDrawAll, boolean isDrawReminder) {
		mLineColor = lineColor;
		mLineWidth = lineWidth;
		isDrawBorder = isDraw;
		isDrawBorderAll = isDrawAll;
		isDrawReminderBorder = isDrawReminder;
	}

	@Override
	protected void dispatchDraw(Canvas canvas) {
		super.dispatchDraw(canvas);
		
		if(isDrawBorder && this.getChildCount() > 0) {
			View localView0 = getChildAt(0);
			int column = getWidth() / localView0.getWidth();
			int childCount = getChildCount();
			
			Paint localPaint = new Paint();
			localPaint.setStyle(Paint.Style.STROKE);
			if(mLineWidth <= 0) {
				mLineWidth = 1;
			}
			localPaint.setStrokeWidth(Utils.defaultToScreen(mLineWidth));
			if(!TextUtils.isEmpty(mLineColor)){
				localPaint.setColor(Color.parseColor(mLineColor));
			} else {
				localPaint.setColor(Color.parseColor("#F8F8F8"));
			}
			
			int[] edgeX = new int[column];
			for(int i = 0; i < childCount; i++) {
				View cellView = getChildAt(i);
				
				if(i < column) {
					edgeX[i] = cellView.getRight();
				}
				
				if(i < column && isDrawBorderAll) {
					//第一行：顶部线
					canvas.drawLine(cellView.getLeft(), cellView.getTop(), cellView.getRight(), cellView.getTop(), localPaint);
				}
				if(i % column == 0  && isDrawBorderAll){
					//第一列：左侧线
					canvas.drawLine(cellView.getLeft(), cellView.getTop(), cellView.getLeft(), cellView.getBottom(), localPaint);
				}//此处不要加else——注意只有1列的情况
				if((i+1) % column == 0) {
					//最后一列：底部线
					if(!isLastRow(childCount, column, i) || isDrawBorderAll) {
						//若不是最后一行，或者是最后一行、但要求画全部线，则画底部线
						canvas.drawLine(cellView.getLeft(), cellView.getBottom(), cellView.getRight(), cellView.getBottom(), localPaint);
					}
					
					if(isDrawBorderAll) {
						//最后一列：右侧线
						canvas.drawLine(cellView.getRight(), cellView.getTop(), cellView.getRight(), cellView.getBottom(), localPaint);
					}
				} else if(isLastRow(childCount, column, i)){
					//最后一行：右侧线
					canvas.drawLine(cellView.getRight(), cellView.getTop(), cellView.getRight(), cellView.getBottom(), localPaint);
					if(isDrawBorderAll) {
						//最后一行：底部线
						canvas.drawLine(cellView.getLeft(), cellView.getBottom(), cellView.getRight(), cellView.getBottom(), localPaint);
					}
				}else {
					//不是最后一行、不是最后一列：右侧线
					canvas.drawLine(cellView.getRight(), cellView.getTop(), cellView.getRight(), cellView.getBottom(), localPaint);
					//不是最后一行、不是最后一列：底部线
					canvas.drawLine(cellView.getLeft(), cellView.getBottom(), cellView.getRight(), cellView.getBottom(), localPaint);
				}
			}
			
			//画出空白部分的网格线
			int reminder = childCount % column;
			if(isDrawReminderBorder && reminder > 0) {
				View lastView = getChildAt(childCount - 1);
				int lastItemWidth = lastView.getWidth();
				int top = lastView.getTop();
				int bottom = lastView.getBottom();
				for(int i = reminder; i < column; i++) {
					int x = edgeX[i];
					if(isDrawBorderAll) {
						//最后一行：底部线
						canvas.drawLine(x-lastItemWidth, bottom, x, bottom, localPaint);
					}
					if(i == (column-1)){
						if(!isDrawBorderAll){
							break;
						} else {
							canvas.drawLine(x, top, x, bottom, localPaint);
						}
					} else {
						canvas.drawLine(x, top, x, bottom, localPaint);
					}
				}
			}
		}
	}
	
	/**
	 * 判断index位置是否为九宫格的最后一行
	 * @param count	九宫格所有item个数
	 * @param column	列数
	 * @param index
	 * @return
	 */
	private boolean isLastRow(int count, int column, int index){
		boolean result = false;
		if(count % column == 0) {
			//刚好整除的情况
			int rows = count / column;
			int lastRowStart = (rows - 1) * column;
			if(index >= lastRowStart) {
				result = true;
			}
		}else{
			//有余数
			int reminder = count % column;
			if(index >= (count - reminder)) {
				result = true;
			}
		}
		
		return result;
	}
}
