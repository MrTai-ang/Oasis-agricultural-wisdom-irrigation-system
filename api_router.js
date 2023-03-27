const express = require('express');
const router = express.Router();

const tempHandler = require('./web_serve')

router.get('/temprouter', tempHandler.getlastedRecord)
router.get('/soilrouter', tempHandler.getlastedsoil)
router.get('/npkrouter', tempHandler.getlastednpk)
router.get('/humrouter', tempHandler.getlastedsoilhum)



module.exports = router;