各科大作业的集合仓库,集中管理避免散乱

添加其他仓库到子目录
> git remote add -f new_work https://github.com/tony-ch/new_work.git  
> git subtree add --prefix=new_work new_work master

分离出分支
> git subtree split -P new_work -b new_work