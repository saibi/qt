.pragma library

function addCall(msg)
{
    var str = msg.toString();
    var res = str.split(",");
    var src1 = parseInt(res[0]);
    var src2 = parseInt(res[1]);
    var result = src1 + src2;

    var data = {
        expression : result
    }

    data.expression = src1 + ' + ' + src2;
    data.result = result;
    return data;
}
