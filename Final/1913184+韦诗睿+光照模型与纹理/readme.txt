计算机图形学期末大作业——照明模型与纹理
南开大学软件学院软件工程专业2019级 韦诗睿 1913184
contact me: 1913184@mail.nankai.edu.cn
github：https://github.com/RuiNov1st/NKU-SE-Computer-Graphics
-------------------

文件夹结构：
- code
    - img: 包含Texture中使用的纹理图片
    - Light: 照明模型
        - Light.cpp: 源代码
        - Light.h: 头文件
        - Light.exe: 可执行文件
    - Texture: 纹理
        - stb_image.h/stb_image_resize.h/stb_image_write.h: Texture中使用的stb_image库。（https://github.com/nothings/stb）
        - Texture.cpp：源代码
        - Texture.exe: 可执行文件
- 1913184+韦诗睿+照明模型与纹理期末大作业报告.pdf
- readme.txt
    

程序执行方法：
- Light.ext：观察光照场景
    相机操作
    - 键盘按'L'/ctrl+'L'：左右移动
    - 键盘按'U'/'D': 上下移动
    - 键盘按'F'/ctrl+'F': 前后移动
    - 键盘按'Y'/ctrl+'Y': 偏航
    - 键盘按'P'/ctrl+'P': 俯仰
    - 键盘按'R'/ctrl+'R':旋转
    材质操作
    - 键盘按'J':材质为红宝石
    - 键盘按'C':材质为铜
    - 键盘按'V':颜色为紫罗兰
    - 键盘按'G':颜色为灰色
    - 键盘按's'(小写):材质为银
    光源操作
    - 键盘按'O'/ctrl+'O': 开关灯
    - 键盘按'M': 光源绑定相机
    - 键盘按'S': 光源固定在场景中
- Texture.exe：观察三种纹理样式在立方体上的效果
    - 键盘按'C'：棋盘纹理
    - 键盘按'N'：图片纹理
    - 键盘按'R'：混合样式

