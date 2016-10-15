#include <ngx_core.h>
#include <ngx_http.h>
#include <ngx_config.h>

static char * ngx_conf_set_myconfig(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r);
static void* ngx_http_mytest_create_loc_conf(ngx_conf_t *cf);
//static ngx_int_t ngx_http_mytest_init(ngx_conf_t *cf);

/*�Զ�������ڴ洢����������Ľṹ��*/
typedef struct{
	ngx_str_t		my_str;
	ngx_int_t		my_num;
	ngx_flag_t		my_flag;
	size_t 			my_size;
	ngx_array_t*	my_str_array;
	ngx_array_t*	my_keyval;
	off_t			my_off;
	ngx_msec_t		my_msec;
	time_t			my_sec;
	ngx_bufs_t		my_bufs;
	ngx_uint_t		my_enum_seq;
	ngx_uint_t		my_bitmask;
	ngx_uint_t		my_access;
	ngx_path_t*		my_path;
}ngx_http_mytest_conf_t;

/*����http������4����֮1:�������ݽṹ���ڴ洢�������Ӧ�Ĳ���*/
static void* ngx_http_mytest_create_loc_conf(ngx_conf_t *cf)
{
	ngx_http_mytest_conf_t *mycf;
	mycf = (ngx_http_mytest_conf_t *)ngx_pcalloc(cf->pool, sizeof(ngx_http_mytest_conf_t));
	if(mycf == NULL)
		return NULL;
	mycf->my_flag = NGX_CONF_UNSET;
	mycf->my_num = NGX_CONF_UNSET;
	mycf->my_str_array = NGX_CONF_UNSET_PTR;
	mycf->my_keyval = NULL;
	mycf->my_off = NGX_CONF_UNSET;
	mycf->my_msec = NGX_CONF_UNSET_MSEC;
	mycf->my_sec = NGX_CONF_UNSET;
	mycf->my_size = NGX_CONF_UNSET_SIZE;
	return mycf;
}


/*����http������4����֮2:�趨��������nginx.conf�г���ʱ������������ص�����
 *ÿ��ngx_command_t�ṹ�嶨�����Լ�����Ȥ��һ��������Ľ�������
 */
static ngx_command_t  ngx_http_mytest_commands[] = {
	{
		/*name*/
		ngx_string("mytest"),
		/*type
		 *��������Գ�����http{},server{},location{},limit_except{}��Щ���ÿ鵱�У��������޲���
		 *ngx_command_t�ṹ����type��Ա��ȡֵ�����壬����p116��4-1
		 */
		//NGX_HTTP_MAIN_CONF|NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_HTTP_LMT_CONF|NGX_CONF_NOARGS,	

		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE12,
		
		/*set 
		 *��������nameָ����������󽫻����set���������������Ĳ���
		 *���������������ַ���:(1)�Զ���ص�����,��4.2.4��.(2)ʹ��nginxԤ���14������������ķ�������4.2.3��.
		 *14������������ķ�������p118��4-2
		 */
		ngx_conf_set_myconfig,	
		/*conf
		 *ʹ��create_loc_conf���������Ľṹ�����洢�����������������
		 *���ֵ�������Ǻ�ngx_http_module_t�ṹ��ʵ�ֵĻص�������ص�
		 */
		NGX_HTTP_LOC_CONF_OFFSET,
		/*offset
		 *���ʹ��Ԥ��Ľ���������ķ������ͱ�������offset��
		 *nginx��ͨ��conf��Ա���ҵ�Ӧ�����ĸ��ṹ������ţ�Ȼ��ͨ��offset���ҵ�����ṹ������Ӧ�ĳ�Ա
		 */
		0,
		NULL
	},
	ngx_null_command
};

/*
 *������ǵ�ģ��û��ʲô�����ұ���Ҫ��HTTP��ܳ�ʼ���׶���ɵ�
 *�Ͳ���ʵ��ngx_http_module_t��8���ص�����
 */
static ngx_http_module_t  ngx_http_mytest_module_ctx = {
	NULL,								/* preconfiguration ���������ļ�ǰ����*/
	//ngx_http_mytest_init,				/* postconfiguration ���������ļ������*/
	NULL,
	NULL,								/* create main configuration */
	NULL,								/* init main configuration */
	NULL,								/* create server configuration */
	NULL,								/* merge server configuration */
	ngx_http_mytest_create_loc_conf,	/* create location configuration */
	NULL								/* merge location configuration */
};

ngx_module_t ngx_http_mytest_module = {
	NGX_MODULE_V1,
	&ngx_http_mytest_module_ctx,	//module context
	ngx_http_mytest_commands,		//module directives
	NGX_HTTP_MODULE,				//module type
	NULL,							//init master
	NULL,							//init module
	NULL,							//init process
	NULL,							//init thread
	NULL,							//exit thread
	NULL,							//exit process
	NULL,							//exit master
	NGX_MODULE_V1_PADDING
};

/*����http������4����֮3:ʵ�ֵ�3���еĻص�������Ҳ����ʹ��nginx���Ԥ���14���ص�����*/
static char * ngx_conf_set_myconfig(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
		/*conf������ngx_http_mytest_create_loc_conf�����д�����ngx_http_mytest_conf_t����*/
		ngx_http_mytest_conf_t *mycf = conf;
		/*cf->args��һ���ַ������飬��������������
		 *value[1]��ʾ��һ��������value[2]��ʾ�ڶ�������
		 */
		ngx_str_t *value = cf->args->elts;
		if(cf->args->nelts < 1)//������һ������
			mycf->my_str = value[1];
		if(cf->args->nelts < 2)//��2������
		{
			mycf->my_num = ngx_atoi(value[2].data, value[2].len);
			if(mycf->my_num == NGX_ERROR)
				return "invalid number";
		}
		//ngx_log_error(NGX_LOG_ALERT, cf->log, 0, "test ngx_log_error by wyy first param = %V, second param = %d\n", mycf->my_str, mycf->my_num);
		//ngx_log_debug(NGX_LOG_DEBUG_HTTP, cf->log, 0, "test ngx_log_debug by wyy first param = %V, second param = %d\n", mycf->my_str, mycf->my_num);
		//ngx_log_error(NGX_LOG_STDERR, cf->log, 0, "test ngx_log_error by wyy first param = %V, second param = %d\n", mycf->my_str, mycf->my_num);

		ngx_http_core_loc_conf_t  *clcf;
		clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);//
		/*������mytestģ���������������nginx������HTTP�����ͷ����Ϣ��
		 *�ͻ����HTTP�������������HTTP��ܹ���11���׶Σ���NGX_HTTP_CONTENT_PHASE
		 *�׶ν��п��ܵ���mytestģ������������Ҳ���ǵ���ngx_http_mytest_handler
		 *
		 *ngx_int_t (*ngx_http_handler_pt)(ngx_http_request_t *r)
		 *�������handler�ص�������ԭ�ͣ�����r��nginx��HTTP��ܽ������û��������
		 *�������ݽṹ�����Դ���ֱ�ӻ�ȡ������ص���Ϣ��
		 *����ֵ������ngx_http_request.h 72�п�ʼ�ĺ�,Ҳ������ngx_core.h 36�п�ʼ��ȫ�ִ�����
		 */
		 
		clcf->handler = ngx_http_mytest_handler;
		return NGX_CONF_OK;
}
#if 0
static ngx_int_t ngx_http_mytest_init(ngx_conf_t *cf)
{
	    ngx_http_core_loc_conf_t  *clcf;
		clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);//
		/*������mytestģ���������������nginx������HTTP�����ͷ����Ϣ��
		 *�ͻ����HTTP�������������HTTP��ܹ���11���׶Σ���NGX_HTTP_CONTENT_PHASE
		 *�׶ν��п��ܵ���mytestģ������������Ҳ���ǵ���ngx_http_mytest_handler
		 *
		 *ngx_int_t (*ngx_http_handler_pt)(ngx_http_request_t *r)
		 *�������handler�ص�������ԭ�ͣ�����r��nginx��HTTP��ܽ������û��������
		 *�������ݽṹ�����Դ���ֱ�ӻ�ȡ������ص���Ϣ��
		 *����ֵ������ngx_http_request.h 72�п�ʼ�ĺ�,Ҳ������ngx_core.h 36�п�ʼ��ȫ�ִ�����
		 */
		 
		clcf->handler = ngx_http_mytest_handler;
		return NGX_OK;
}
#endif

static ngx_int_t ngx_http_mytest_handler(ngx_http_request_t *r)
{
	/*ֻ֧��GET �� HEAD����*/
	if(!(r->method & (NGX_HTTP_GET | NGX_HTTP_HEAD)))
		return NGX_HTTP_NOT_ALLOWED;
	/*���������еİ���*/
	ngx_int_t rc = ngx_http_discard_request_body(r);
	if(rc != NGX_OK)
		return rc;

	/*����HTTPͷ�����Լ�����*/
	ngx_str_t type = ngx_string("text/plain");
	//ngx_str_t response = ngx_string("Hello World!");
	r->headers_out.status = NGX_HTTP_OK;
	//r->headers_out.content_length_n = response.len;
	r->headers_out.content_type = type;
	/*֧���ļ��Ķϵ�����*/
	r->allow_ranges = 1;
	/*����ngx_buf_t׼�����Ͱ���,���buf�������ļ�*/
	ngx_buf_t *b = NULL;
	b = ngx_palloc(r->pool, sizeof(ngx_buf_t));
	u_char* filename = (u_char*)"/tmp/test.txt";
	b->in_file = 1;
	b->file = ngx_palloc(r->pool, sizeof(ngx_file_t));
	b->file->fd = ngx_open_file(filename, NGX_FILE_RDONLY|NGX_FILE_NONBLOCK, NGX_FILE_OPEN, 0);
	b->file->log = r->connection->log;
	b->file->name.data = filename;
	b->file->name.len = strlen((const char *)filename);
	b->file_pos = 0;
	b->file_last = b->file->info.st_size;
	if(b->file->fd <= 0)
		return NGX_HTTP_NOT_FOUND;
	if(ngx_file_info(filename, &b->file->info) == NGX_FILE_ERROR)
		return NGX_HTTP_INTERNAL_SERVER_ERROR;

	r->headers_out.content_length_n = b->file->info.st_size;
	b->last_buf = 1;
	/*���췢�Ͱ���ʱ��ngx_chain_t�ṹ��*/
	ngx_chain_t out;
	out.buf = b;
	out.next = NULL;
	
	/*����HTTPͷ��*/
	rc = ngx_http_send_header(r);



	/*��Ϊ��Ӧ����Ϣbody�Ļ��������д��ļ������������������ʱ��һ��
	 *Ҫ�����ļ��������������������ڴ�ص��еģ����ڴ�����������ڽ�����ʱ���
	 *��������Ҫ����Ķ���Ļص�����(������C++��������)����������Ҫ���ľ���ʵ����
	 *һ���ڴ���������������Ļص�����Ϊngx_pool_cleanup_file(ngxʵ�ֵ�ר����
	 *�������ڴ�����ļ��ĺ���)Ȼ����䴫�ݸ��ص������Ĳ�����
	 */
	/*һ��Ҫ���������cleaner�ͺͱ������junk����������Ĺ�ϵ*/
	/*���ڴ�����һ���������cleaner*/
	ngx_pool_cleanup_t *cleaner = ngx_pool_cleanup_add(r->pool, sizeof(ngx_pool_cleanup_file_t));
	/*����������Ļص�����Ϊר�������ļ���ngx_pool_cleanup_file����*/
	cleaner->handler = ngx_pool_cleanup_file;
	/*����������cleaner�Ļص������Ĳ�����Ҳ���ǵȴ�������Ķ���junk*/
	ngx_pool_cleanup_file_t *junk = cleaner->data;
	/*���ȴ�������Ķ���junk*/
	junk->fd = b->file->fd;
	junk->log = r->pool->log;
	junk->name = b->file->name.data;

#if 0

	/*����ngx_buf_t׼�����Ͱ���*/
	ngx_buf_t *body_buf = NULL;
	body_buf = ngx_create_temp_buf(r->pool, response.len);
	if(body_buf == NULL)
		return NGX_HTTP_INTERNAL_SERVER_ERROR;
	/*���������ݿ�����body_buf��ȥ*/
	ngx_memcpy(body_buf->pos, response.data, response.len);
	/*pos �� last֮���ʾ�����������ݣ�����һ��Ҫ���ú�last�������Ƿ��Ͳ���ȥ��*/
	body_buf->last = body_buf->pos + response.len;
	/*��ʾ�������һ�黺����*/
	body_buf->last_buf = 1;

#endif

	/*���һ��Ϊ���Ͱ��壬 ���ͽ�����HTTP��ܻ����ngx_http_finalize_request��������*/
	return ngx_http_output_filter(r, &out);
}



/*����http������4����֮4:�ϲ���ͬ��������ÿ��г��ֵ�ͬ��������*/




