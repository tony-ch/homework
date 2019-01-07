GANPaint()类包装了上传图片的功能,初始化需要提供一个json文件的路径,用来保存预定义的操作对应的ablations值,参见preset_ops.json

使用上传功能调用GANpaint()的upload参数即可,如下,700是图片id,operations是操作序列,用list保存,返回numpy数组,方便直接操作, 更多detail参见upload函数
np_img = ganpaint.upload(700,openrations)

operations的定义: 
operations = [op1,op2,...]
op = {'op':op_name, 'mask': mask img}
op_name: str, like 'add_tree','remove_tree',... //所有操作参见preset_ops.json
mask: (RGBA img, 32x32x4 numpy array)

另注: 经过测试, mask里面的颜色似乎并不影响,remove操作的时候上传绿色mask也可以,draw操作的时候上传红色mask也可以