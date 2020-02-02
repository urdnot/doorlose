let backend = require('bindings')('backend');
let st = backend.initialize();
let st1 = backend.add_task(0, "Test task");
let st2 = backend.get_task(backend.UNDEFINED_CLIENT_ID, 0);
console.log(st2.status);
console.log(st2.message);
console.log(st2.client_id);