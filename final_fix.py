import json, os, shutil

fpath = 'nodered_flow_extended.json'
deploy = os.path.expanduser('~/.node-red/flows.json')

with open(fpath, 'r', encoding='utf-8') as f:
    d = json.load(f)

for node in d:
    if node['id'] == 'nr_accum':
        f = node['func']
        
        # Fix: 2D array init instead of fill(0)
        old_init = "var radarData = flow.get('radarData') || new Array(181).fill(0);"
        new_init = "var radarData = flow.get('radarData');if(!radarData||typeof radarData[0]!=='object'){radarData=[];for(var ri=0;ri<=180;ri++)radarData[ri]=[];}"
        if old_init in f:
            f = f.replace(old_init, new_init)
            print('Fixed init: 2D arrays')
        else:
            print('Init not found:', old_init[:50])
        
        # Fix: push instead of overwrite
        old_assign = "if (angle >= 0 && angle <= 180) { radarData[angle] = distance; }"
        new_assign = "if(angle>=0&&angle<=180){if(!Array.isArray(radarData[angle]))radarData[angle]=[];if(distance>2){radarData[angle].push(distance);if(radarData[angle].length>5)radarData[angle].shift();}}"
        if old_assign in f:
            f = f.replace(old_assign, new_assign)
            print('Fixed: push instead of overwrite')
        
        # Fix out1: deep copy for chart
        old_out1 = "var out1 = {payload: {radarData: radarData.slice()"
        new_out1 = "var od=[];for(var ii=0;ii<=180;ii++){od[ii]=Array.isArray(radarData[ii])?radarData[ii].slice():(radarData[ii]>0?[radarData[ii]]:[]);}var out1 = {payload: {radarData: od"
        if old_out1 in f:
            f = f.replace(old_out1, new_out1)
            print('Fixed out1: deep copy')
        
        node['func'] = f

with open(fpath, 'w', encoding='utf-8', newline='') as f:
    json.dump(d, f, ensure_ascii=False, indent=4)

shutil.copy(fpath, deploy)
print('Deployed - restart Node-RED')
