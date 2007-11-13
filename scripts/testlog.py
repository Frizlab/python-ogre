import logging
import logger
logger1 = logging.getLogger('myapp.area1')
logger2 = logging.getLogger('myapp.area2')
import sys
print sys.argv[0]
print sys.argv[0].split('.')[0]

logger1.debug('Quick zephyrs blow, vexing daft Jim.')
logger1.info('How quickly daft jumping zebras vex.')
logger2.warning('Jail zesty vixen who grabbed pay from quack.')
logger2.error('The five boxing wizards jump quickly.')

