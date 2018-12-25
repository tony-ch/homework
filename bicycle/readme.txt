安装mysql和mysql-client
安装mysql-workbench

> sudo pacman -S mysql mysql-client mysql-workbench
> sudo mysqld --initialize --user=mysql --basedir=/usr --datadir=/var/lib/mysql
> sudo systemctl start mysqld
> sudo mysqladmin -u root -p _&v4rwLaUS?% password "root"

ubuntu上
> sudo apt install mysql-server mysql-client libmysqlclient-dev mysql-workbench

用workbench 创建 schema,名字为bicycle

用命令行
> mysql -u root -p tony1234
> CREATE DATABASE bicycle;
> use bicycle;
> source /pathto/bicycle.sql;

安装mysql8.0或更高版本需要在mysql中运行
> alter user root@localhost identified with mysql_native_password by 'root';

安装tomcat8
> sudo pacman -S tomcat8
> vim /etc/tomcat8/tomcat-users.xml   //更改密码 ubuntu上在/var/lib/tomcat8
> sudo gpasswd -a tony tomcat
> sudo systemctl start tomcat8      //ubuntu sudo servie tomcat8 start
配置https
创建JKS证书
> keytool -genkey -v -alias testKey -keyalg RSA -validity 3650 -keystore ~/testKey.keystore
编辑/etc/tomcat8/server.xml，将下面部分取消注释，添加路径和口令     //ubuntu在/var/lib/tomcat8/conf/
    <Connector port="8443" protocol="org.apache.coyote.http11.Http11Protocol"
               maxThreads="150" SSLEnabled="true" scheme="https" secure="true"
               clientAuth="false" sslProtocol="TLS" keystoreFile="你的keystore路径" keystorePass="生成证书时的口令"  />


IDEA创建工程需要一些配置
Module设置(File>Project Structure>Module>+(新建)>Web Module)
   Deploy Descriptor的path --> /pathto/bicycle/WebContent/WEB-INF/web.xml, 
   Web Resource Directory  --> /pathto/bicycle/WebContent
   Path Relative to Deploy Path --> /
配置tomcat服务器
   新建Template(Run/Debug Configurations>Templates>Tomcat Server>Local)
      tomcat_home --> /usr/share/tomcat8
      ubuntu 上 tomcat base directory -> /var/lib/tomcat8
      权限问题报错的话需要运行
      > sudo chmod 777 -R /usr/share/tomcat8/conf
      ubuntu上sudo chmod 777 -R /var/lib/tomcat8/conf
   创建配置(Run/Debug Configurations>+(新建)>Tomcat Server>Local)
   Deployment>Application Context --> /bicycle
   需要将Tomcat添加到Module的依赖中(File>Project Structure>Module>Dependence)
Library设置
   /pathto/bicycle/WebContent/WEB-INF/lib/下的4个jar文件,需要将它们加到Module的依赖里面
Artifacts设置
   配置Tomcat服务器的时候会有配置Artifacts的提示


> pip3 install face_recognition flask
安装dlib依赖的时候,如果gcc版本是5.5可能会报错,使用
for f in avx512fintrin.h avx512pfintrin.h avx512vlintrin.h; do
   curl -H "User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36" -o $f "https://gcc.gnu.org/viewcvs/gcc/branches/gcc-5-branch/gcc/config/i386/${f}?view=co&revision=245536&content-type=text%2Fplain&pathrev=245536"
done && sudo mv avx512*intrin.h  /usr/lib/gcc/x86_64-linux-gnu/5/include/
可以解决

需要运行WebContent/py/face_server.py
> python3 face_server.py

把编译部署的目录加到/etc/tomcat8/server.xml里实现部署(还有其他部署方式),在<Host name="localhost"  appBase="webapps"...</Host>中加入
<Context path="/bicycle" docBase="/pathto/bicycle/out/artifacts/bicycle_Web_exploded"></Context>