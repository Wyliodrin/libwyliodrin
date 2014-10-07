"use strict"

var c = require('./wyliodrin_communication');
c.initCommunication(6379);
c.sendMessage("ioana.culic_pixy@wyliodrin.com",1, "acesta e un test");
c.closeCommunication();