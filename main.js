let express = require('express');
let backend = require('bindings')('backend');
let app = express();

const CATEGORIES = {
    "love": 0,
    "rest": 1,
    "will": 2,
    "family": 3,
    "body": 4,
    "work": 5
};

app.get('/tasks', function(req, res)
{
    let result = {};
    result.client_id = backend.UNDEFINED_CLIENT_ID;
    result.tasks = {};

    if (typeof req.query.categories === "undefined")
    {
        throw new Error("Request does not have query");
    }

    const client_id_type = typeof req.query.client_id;
    console.log(client_id_type);
    if (client_id_type === "undefined")
    {
        // do nothing
    }
    else if (client_id_type === "string")
    {
        const num = Number.parseInt(req.query.client_id);
        console.log(num);
        if (!Number.isNaN(num) &&
            (num >= 0 || num < backend.UNDEFINED_CLIENT_ID))
        {
            result.client_id = num;
        }
        else
        {
            throw new Error(`Client id is invalid ` +
            `assert(${num} >= 0 && ` +
            `${num} < ${backend.UNDEFINED_CLIENT_ID})`);
        }
    }
    else
    {
        throw new Error(`Client id must be undefined or string. ` +
        `assert(${client_id_type} === undefined || ${client_id_type} === string)`);
    }

    if (req.query.categories.length == 0 || req.query.categories.length > 3)
    {
        throw new Error("Categories array has invalid size " + req.query.categories.length);
    }
        
    for (let i = 0; i < req.query.categories.length; ++i)
    {
        if (CATEGORIES[req.query.categories[i]] === "undefined")
        {
            throw new Error(`Category name ${req.query.categories[i]} is invalid`);
        }
    }

    for (let i = 0; i < req.query.categories.length; ++i)
    {
        console.log(result.client_id);
        let tmp = backend.get_task(result.client_id, CATEGORIES[req.query.categories[i]]);
        result.client_id = tmp.client_id;
        result.tasks[i] = {
            "category": req.query.categories[i],
            "task": tmp.message
        };
    }

    res.json(result);
});

let init = function()
{
    backend.initialize();
};

let add_tasks = function()
{
    for (let i = 0; i < 6; ++i)
    {
        for (let j = 0; j < 100; ++j)
        {
            let text = "This is a " + j + " test text from " + i + " category.";
            backend.add_task(i, text);
        }
    }
};

app.listen(3000, function()
{
    init();
    add_tasks();
    console.log('Doorlose initialized, test tasks added. Server listen 3000 port');
});

// process.on('SIGTERM', function()
// {
//     app.close(function()
//     {
//         console.log('Process terminated');
//     });
// });