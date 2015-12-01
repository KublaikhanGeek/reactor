HtmlParser是一个基于SAX方法的html页面的解析器。

SAX的全称是Simple API for XML，它是一种流式的解析XML文件的方法。它的大致思想是用户向解析器注册一些回调函数，解析器在解析页面的过程中，当特定的事件发生的时候，调用对应的回调函数，完成用户指定的任务。想要详细了解SAX的读者朋友，可以参考：<a href='http://en.wikipedia.org/wiki/Simple_API_for_XML'><a href='http://en.wikipedia.org/wiki/Simple_API_for_XML'>http://en.wikipedia.org/wiki/Simple_API_for_XML</a></a>。

要使用HtmlParser, 具体有以下三步：<br />
（1）包含htmlparser.h<br />
（2）实现自己的EventHandler<br />
（3）把自己的EventHandler注册到HtmlParser中，调用Parse()方法启动parser<br />

详细的用法参见：<a href='http://www.wuzesheng.com/?p=1417'><a href='http://www.wuzesheng.com/?p=1417'>http://www.wuzesheng.com/?p=1417</a></a>，如果在使用的过程中发现任何问题，都可以在博客上留言，谢谢！<br />