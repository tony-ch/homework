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


IDEA配置tomcat服务器
tomcat_home 填写 /usr/share/tomcat8
ubuntu 上tomcat base directory 填写/var/lib/tomcat8
权限问题报错的话需要运行
> sudo chmod 777 -R /usr/share/tomcat8/conf
ubuntu上sudo chmod 777 -R /var/lib/tomcat8/conf

> pip3 install face_recognition
安装dlib依赖的时候,如果gcc版本是5.5可能会报错,使用
for f in avx512fintrin.h avx512pfintrin.h avx512vlintrin.h; do
   curl -H "User-Agent:Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36" -o $f "https://gcc.gnu.org/viewcvs/gcc/branches/gcc-5-branch/gcc/config/i386/${f}?view=co&revision=245536&content-type=text%2Fplain&pathrev=245536"
done && sudo mv avx512*intrin.h  /usr/lib/gcc/x86_64-linux-gnu/5/include/
可以解决
