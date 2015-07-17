//
//  MyLayoutBase.h
//  MyLinearLayoutDemo
//
//  Created by apple on 15/6/14.
//  Copyright (c) 2015年 SunnadaSoft. All rights reserved.
//

#import <UIKit/UIKit.h>


//视图的停靠属性
typedef enum : unsigned char {
    MGRAVITY_NONE = 0,
    
    //水平
    MGRAVITY_HORZ_LEFT = 1,
    MGRAVITY_HORZ_CENTER = 2,
    MGRAVITY_HORZ_RIGHT = 4,
    //水平填满
    MGRAVITY_HORZ_FILL = MGRAVITY_HORZ_LEFT | MGRAVITY_HORZ_CENTER | MGRAVITY_HORZ_RIGHT,
    
    //垂直
    MGRAVITY_VERT_TOP = 1 << 4,
    MGRAVITY_VERT_CENTER = 2 << 4,
    MGRAVITY_VERT_BOTTOM = 4 << 4,
    //垂直填满
    MGRAVITY_VERT_FILL = MGRAVITY_VERT_TOP | MGRAVITY_VERT_CENTER | MGRAVITY_VERT_BOTTOM,
    
    //居中
    MGRAVITY_CENTER = MGRAVITY_HORZ_CENTER | MGRAVITY_VERT_CENTER,
    
    //全屏填满
    MGRAVITY_FILL = MGRAVITY_HORZ_FILL | MGRAVITY_VERT_FILL
    
} MarignGravity;



/*布局位置对象*/
@interface MyLayoutPos : NSObject

//偏移
-(MyLayoutPos* (^)(CGFloat val))offset;

//NSNumber, MyLayoutPos对象,如果是centerXPos或者centerYPos则可以传NSArray，数组里面里面也必须是centerXPos，表示指定的视图数组
//在父视图中居中，比如： A.centerXPos.equalTo(@[B.centerXPos.offset(20)].offset(20)
//表示A和B在父视图中居中往下偏移20，B在A的右边，间隔20。
//如果是NSNumber值则表示离布局视图的特定方向的边界值，对于线性布局来说如果大于0小于1则表示间距是相对值。
//如果是MyLayoutPos则是相对于另外一个视图的边界值
//如果是NSArray则是视图成组后的值。
-(MyLayoutPos* (^)(id val))equalTo;

@end


/*布局尺寸对象*/
@interface MyLayoutDime : NSObject

//乘
-(MyLayoutDime* (^)(CGFloat val))multiply;

//加,用这个和equalTo的数组功能可以实现均分子视图宽度以及间隔的设定。
-(MyLayoutDime* (^)(CGFloat val))add;


//NSNumber, MyLayoutDime以及MyLayoutDime数组，数组的概念就是所有数组里面的子视图的尺寸平分父视图的尺寸。
-(MyLayoutDime* (^)(id val))equalTo;


@end


@interface UIView(MyLayoutExt)

//边界位置
@property(nonatomic, readonly)  MyLayoutPos *leftPos;
@property(nonatomic, readonly)  MyLayoutPos *topPos;
@property(nonatomic, readonly)  MyLayoutPos *rightPos;
@property(nonatomic, readonly)  MyLayoutPos *bottomPos;
@property(nonatomic, readonly)  MyLayoutPos *centerXPos;
@property(nonatomic, readonly)  MyLayoutPos *centerYPos;


//尺寸,如果设置了尺寸则以设置尺寸优先，否则以视图自身的frame的高宽为基准
@property(nonatomic, readonly)  MyLayoutDime *widthDime;
@property(nonatomic, readonly)  MyLayoutDime *heightDime;

//设定视图的高度在宽度是固定的情况下根据内容的大小而浮动,如果内容无法容纳的话则自动拉升视图的高度,如果原始高度高于内容则会缩小视图的高度。默认为NO, 这个属性主要用UILabel,UITextView的多行的情况。
@property(nonatomic, assign, getter=isFlexedHeight) BOOL flexedHeight;


@end



/**画线用于布局的四周的线的绘制**/
@interface MyBorderLineDraw : NSObject

@property(nonatomic) UIColor *color;             //颜色
@property(nonatomic) UIColor *insetColor;        //嵌入颜色，用于实现立体效果
@property(nonatomic,assign) CGFloat thick;       //厚度,默认为1
@property(nonatomic,assign) CGFloat headIndent;  //头部缩进
@property(nonatomic, assign) CGFloat tailIndent;  //尾部缩进
@property(nonatomic, assign) CGFloat dash;        //虚线的点数如果为0则是实线。

-(id)initWithColor:(UIColor*)color;

@end


/*布局基类，基类不支持实例化对象*/
@interface MyLayoutBase : UIView

@property(nonatomic,assign) UIEdgeInsets padding;  //用来描述里面的子视图的离自己的边距，默认上下左右都是0
//这个是上面属性的简化设置版本。
@property(nonatomic, assign) CGFloat topPadding;
@property(nonatomic, assign) CGFloat leftPadding;
@property(nonatomic, assign) CGFloat bottomPadding;
@property(nonatomic, assign) CGFloat rightPadding;


//高宽是否由子视图决定，这两个属性只对线性和相对布局有效，框架布局无效
@property(nonatomic,assign) BOOL wrapContentWidth;
@property(nonatomic,assign) BOOL wrapContentHeight;


//如果布局的父视图是UIScrollView或者子类则在布局的位置调整后是否调整滚动视图的contentsize,默认是NO
//这个属性适合与整个布局作为滚动视图的唯一子视图来使用。
@property(nonatomic, assign, getter = isAdjustScrollViewContentSize) BOOL adjustScrollViewContentSize;



//布局时是否调用基类的布局方法，这个属性设置的作用体现在当自身的高度或者宽度调整时而且里面的子视图又设置了autoresizingMask时
//优先进行子视图的位置和高度宽度的拉升缩放，属性默认是NO
@property(nonatomic, assign) BOOL priorAutoresizingMask;


//指定当子视图隐藏时是否重新布局，默认为YES，表示一旦子视图隐藏则关联视图会重新布局。如果设置为NO的话则隐藏的子视图是不会重新布局的。
@property(nonatomic, assign) BOOL hideSubviewReLayout;


//设置自动布局前后的处理块，主要用于动画处理，可以在这两个函数中添加动画的代码。,如果为nil
@property(nonatomic,copy) void (^beginLayoutBlock)();
@property(nonatomic,copy) void (^endLayoutBlock)();

//当前是否正在布局中。
@property(nonatomic,assign) BOOL isLayouting;


//指定四个边界线的绘制。
@property(nonatomic, strong) MyBorderLineDraw *leftBorderLine;
@property(nonatomic, strong) MyBorderLineDraw *rightBorderLine;
@property(nonatomic, strong) MyBorderLineDraw *topBorderLine;
@property(nonatomic, strong) MyBorderLineDraw *bottomBorderLine;

//同时设置4个边界线。
@property(nonatomic, strong) MyBorderLineDraw *boundBorderLine;

//下面的功能支持在布局视图中的触摸功能。

//高亮的背景色,我们支持在布局中执行单击的事件，用户按下时背景会高亮.只有设置了事件才会高亮
@property(nonatomic,strong) UIColor *highlightedBackgroundColor;

//设置单击触摸的事件，如果target为nil则取消事件。
-(void)setTarget:(id)target action:(SEL)action;





//内部使用函数，外部不需要调用。。
-(void)construct;

//派生类重载这个函数进行布局
-(void)doLayoutSubviews;

//判断margin是否是相对margin
-(BOOL)isRelativeMargin:(CGFloat)margin;


-(void)calcMatchParentWidth:(MyLayoutDime*)match selfWidth:(CGFloat)selfWidth leftMargin:(CGFloat)leftMargin rightMargin:(CGFloat)rightMargin leftPadding:(CGFloat)leftPadding rightPadding:(CGFloat)rightPadding rect:(CGRect*)pRect superView:(UIView*)newSuperview;

-(void)calcMatchParentHeight:(MyLayoutDime*)match selfHeight:(CGFloat)selfHeight topMargin:(CGFloat)topMargin bottomMargin:(CGFloat)bottomMargin topPadding:(CGFloat)topPadding bottomPadding:(CGFloat)bottomPadding rect:(CGRect*)pRect superView:(UIView*)newSuperview;





@end
