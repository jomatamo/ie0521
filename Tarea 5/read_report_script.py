#!/usr/bin/python
import re
import glob



modules=[]

def read_report(type,path,name,module):

    module['Name']=name
    module['Type']=type
    time_group = False

    report = open(path,"r")
    for line in report:
        #Se buscan primero los nombres de los timing groups y los slacks
        searchObj = re.search( r'Timing\sPath\sGroup\s*\'(.*clk.*)\'', line)

        if searchObj:
            time_group=True
            tg={'Name':'','NegSlack':'','Slack':''} #se crea un hash para ir guardando valores
            tg['Name']= searchObj.group(1)
        if time_group==True:
            searchObj = re.search( r'Critical\sPath\sSlack:\s*(.*)', line)
            if searchObj:
                tg['Slack'] = searchObj.group(1)
            searchObj = re.search( r'Total\sNegative\sSlack:\s*(.*)', line)
            if searchObj:
                tg['NegSlack'] = searchObj.group(1)
                module['Module_stats']['timing_groups'].append(tg)
                time_group=False
        #Se buscan la cantidad de celdas
        searchObj = re.search( r'Combinational\sCell\sCount:\s*(.*)', line)
        if searchObj:
            cll={'Seq_cnt':'','Comb_cnt':''}
            cll['Comb_cnt']= searchObj.group(1)
        searchObj = re.search( r'Sequential\sCell\sCount:\s*(.*)', line)
        if searchObj:
            cll['Seq_cnt']= searchObj.group(1)
            module['Module_stats']['Cell_cnt']=cll
        #Se busca el area
        searchObj = re.search( r'Combinational\sArea:\s*(.*)', line)
        if searchObj:
            ar={'Comb_area':'','Seq_area':'','Dsgn_area':''}
            ar['Comb_area']= searchObj.group(1)
        searchObj = re.search( r'Noncombinational\sArea:\s*(\S+)\n', line)
        if searchObj:
            ar['Seq_area']= searchObj.group(1)
        searchObj = re.search( r'Noncombinational\sArea:\s*\n', line)
        if searchObj:
            searchObj2= re.search(r'^\s*(.*)', report.next())
            ar['Seq_area']= searchObj2.group(1)
        searchObj = re.search( r'Design\sArea:\s*(.*)\n', line)
        if searchObj:
            ar['Dsgn_area']= searchObj.group(1)
            area=float(ar['Dsgn_area'])
            module['Module_stats']['Area']=ar
    report.close()
    return area

def cat_modules(a_percent,module,modules):
    print_other = True
    for i in module['Module_stats']['timing_groups']:
        cat=""
        cat=cat+module['Name']+','
        cat=cat+module['Type']+','
        cat=cat+i['Name']+','
        cat=cat+i['Slack']+','
        cat=cat+i['NegSlack']+','
        if print_other:
            cat=cat+module['Module_stats']['Cell_cnt']['Comb_cnt']+','
            cat=cat+module['Module_stats']['Cell_cnt']['Seq_cnt']+','
            cat=cat+module['Module_stats']['Area']['Comb_area']+','
            cat=cat+module['Module_stats']['Area']['Seq_area']+','
            cat=cat+module['Module_stats']['Area']['Dsgn_area']+','
            cat=cat+str(a_percent)
            modules.append(cat)
            print_other = False
        else:
            cat=cat+''+','
            cat=cat+''+','
            cat=cat+''+','
            cat=cat+''+','
            cat=cat+''+','
            cat=cat+''
            modules.append(cat)


    return


for pathhier in glob.iglob('./hier/*.qor.rpt'):

    pathflat = re.sub(r'hier','flat',pathhier)
    
    searchObj = re.search(r'.\/hier[\\|\/](.*)\.qor.rpt*',pathhier)
    if searchObj:
        name = searchObj.group(1)
    module1={'Name':'','Type':'','Module_stats': {'timing_groups':[],'Cell_cnt':{},'Area':{}}}
    module2={'Name':'','Type':'','Module_stats': {'timing_groups':[],'Cell_cnt':{},'Area':{}}}

    a_hier=read_report("hier",pathhier,name,module1)
    a_flat=read_report("flat",pathflat,name,module2)
    a_percent=100*((a_hier-a_flat)/a_hier)
    cat_modules(a_percent,module1,modules)
    cat_modules(a_percent,module2,modules)


report = open("stats.csv","w")
report.write("module,syn mode,clk,slack,TNS,Comb cell count,Seq cell count,Comb area, Seq area,Design Area,% area impr"+"\n")
for i in modules:
        report.write(i+"\n")
report.close()
