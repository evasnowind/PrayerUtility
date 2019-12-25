
import sys
import json
import datetime

from torndb import Connection

# 测试环境
db_config = dict(
    host='数据库ip地址:端口号',
    db='数据库名称',
    user='用户名',
    password='密码',
)


def do_something():
    db = Connection(host=db_config['host'], database=db_config['db'],
                         user=db_config['user'], password=db_config['password'])
    # 利用db对象进行CRUD操作即可。操作实例可以参考refresh_auth_new_module函数
    # Connection支持的操作参见 https://github.com/bdarnell/torndb
    # 文档地址：https://torndb.readthedocs.io/en/latest/

    # 操作结束后记得关闭数据库连接
    db.close()


def refresh_auth_new_module(project_code, role_id, module_url):
    auth_db = Connection(host=db_config['host'], database=db_config['db'],
                        user=db_config['user'], password=db_config['password'])
    module_url_str = "/" + module_url
    query_result = auth_db.query('SELECT * FROM t_module WHERE curl=%s AND state=1 AND project_code=%s',
                                module_url_str, project_code)
    module_id = -1
    for info in query_result:
        print("即将刷新：" + project_code + "系统 功能：" + info.name + " url=" + info.curl)
        module_id = info.module_id

    user_info = auth_db.query("SELECT user_name, real_name FROM t_user WHERE project_code=%s AND user_id in "
                              "(SELECT user_id FROM t_user_role WHERE role_id=%s)",
                              project_code, role_id)
    role_info = auth_db.query("SELECT role_name FROM t_role WHERE role_id=%s", role_id)


    print("即将为如下用户新增权限数据：")
    for user in user_info:
        print("用户名：" + user.real_name + "(" + user.user_name + ")" + " 角色：" + role_info[0].role_name)

    user_input = input("请确认是否刷新？[yes/no]")
    if user_input == "yes":
        print("已确认，开始刷新...")
        print("执行SQL语句：INSERT INTO t_user_module (user_id,module_id) "
              "SELECT user_id,%s as module_id  FROM t_user_role WHERE role_id in(%s)" % (module_id, role_id))
        # 下面这种输出是python 3 print新特性
        # print("执行SQL语句：INSERT INTO t_user_module (user_id,module_id) "
        #       f"SELECT user_id,{module_id} as module_id  FROM t_user_role WHERE role_id in({role_id})")
        auth_db.insert("INSERT INTO t_user_module (user_id,module_id) "
                       "SELECT user_id,%s as module_id  FROM t_user_role WHERE role_id in(%s)", module_id, role_id)
        print("SQL执行结束。")
    else:
        print("未确认，退出！")
    auth_db.close()


def main():
    project_code = sys.argv[1]
    role_id = sys.argv[2]
    module_url = sys.argv[3]
    print("project_code=" + project_code + " role_id=" + role_id + " module_url=" + module_url)
    refresh_auth_new_module(project_code, role_id, module_url)
    print("执行结束！")


if __name__ == '__main__':
    main()

