from types import FrameType
from typing import List
import xml.etree.ElementTree as ET
import logging
import argparse
import os
import sys


# create logger
logging.getLogger('SM_GENERATOR')
logging.basicConfig(format='%(funcName)s::%(levelname)s::%(message)s', level=logging.DEBUG)

# Create the parser
my_parser = argparse.ArgumentParser(prog = 'Statemachine generator',
                                    description='Create the header and source file of a application statemachine')

stateNames = []
eventNames = []
stateList = []
sourceFile = ""
headerFile = ""
stateHandlingFunctionLists = []

def getSmName():
    global root
    return root.attrib['name']

def getStateData(stateNode):
    stateDict = {}
    for eventNode in stateNode:
        logging.debug("Get an event node")
        logging.debug("# name: {}".format(eventNode.attrib["name"]))
        
        function = eventNode.find("Function")
        if function is None:
            logging.error("missing function element")
            return False

        nextstate = eventNode.find("NextState")
        if nextstate is None:
            logging.error("missing nextstate element")
            return False

        logging.debug("# function: {}".format(function.text))
        logging.debug("# next state: {}".format(nextstate.text))
        stateDict.update({eventNode.attrib["name"]: [function.text, nextstate.text]})
    logging.debug("number of events: {}".format(len(stateDict)))

    if stateDict not in stateList:
        stateList.append(stateDict)
    return True

def getEvent():
    global root
    for state in root:
        for event in state:
            if event.attrib['name'] not in eventNames:
                eventNames.append(event.attrib['name'])

def getState():
    global root
    for state in root:
        if state.attrib['name'] not in stateNames:
            logging.debug("Get an state node")
            logging.debug("# name: {}".format(state.attrib['name']))
            stateNames.append(state.attrib['name'])
            if getStateData(state) is False:
                return False
    return True

def generateTypedefEnum(valuesList : List, isEvent : bool):
    with open(sourceFile, "a") as file:
        file.write("/** @brief definition TODO\n *\n */\n")
        file.write("typedef enum\n")
        file.write("{\n")
        for element in valuesList:
            name = "\t{},\n".format(element)
            file.write(name.upper())
            file.write("\t/**< TODO comment*/\n\n")
        if isEvent:
            file.write("\tNUM_OF_EVENT,\n".format(element))
            file.write("}ENUM_EVENT;\n")
        else:
            file.write("\tSTATE_ERROR,\n".format(element))
            file.write("\tNUM_OF_STATE,\n".format(element))
            file.write("}ENUM_STATE;\n")
        file.write("\n")
    pass

def generateStatemachineStruct(SmName : str):
    global sourceFile
    with open(sourceFile, "a") as file:
        file.write("static EzmStateMachine st{};\n".format(SmName))
        file.write("\n")

def generateStateHandle(stateName : List):
    global sourceFile
    with open(sourceFile, "a") as file:
        file.write("/* State Handling section *****************************************************/\n")
        for name in stateName:
            tempList = []
            file.write("static uint8_t Handle{} (uint8_t eEvent, void * pstData);\n".format(name))
            file.write("static void Handle{}Entry (void);\n".format(name))
            file.write("static void Handle{}Exit (void);\n".format(name))
            file.write("\n")
            tempList.append("Handle{}".format(name))
            tempList.append("Handle{}Entry".format(name))
            tempList.append("Handle{}Exit".format(name))
            stateHandlingFunctionLists.append(tempList)
        file.write("static void HandleStateMachineError(void);\n")
        file.write("static uint8_t GetExternalEvent (void);\n")
        file.write("\n")

def generateStateArray(stateNames : list, functionNames : list):
    global sourceFile
    with open(sourceFile, "a") as file:
        file.write("static EzmState astState[NUM_OF_STATE] = \n")
        file.write("{\n")
        index = 0
        for name in stateNames:
            file.write("\t{ ")
            file.write("\"{}\"".format(name))
            if index < len(stateHandlingFunctionLists):
                for fName in stateHandlingFunctionLists[index]:
                    file.write(", &{}".format(fName))
                file.write("},\n")
                index = index + 1
            else:
                logging.error("generate state array error!!!")

        file.write("};\n")
        file.write("\n")

def generateInitFunction(smName):
    global sourceFile
    with open(sourceFile, "a") as file:
        file.write("void Init{}(void)\n".format(smName))
        file.write("{\n")
        file.write("\tezmStateMachine_Init(&st{},\n".format(smName))
        file.write("\t\t\t\t\t\t\t{},\n".format(eventNames[0].upper()))
        file.write("\t\t\t\t\t\t\t{},\n".format(stateNames[0].upper()))
        file.write("\t\t\t\t\t\t\tNUM_OF_EVENT,\n")
        file.write("\t\t\t\t\t\t\tNUM_OF_STATE,\n")
        file.write("\t\t\t\t\t\t\tastState,\n")
        file.write("\t\t\t\t\t\t\t&HandleStateMachineError,\n")
        file.write("\t\t\t\t\t\t\t&GetExternalEvent,\n")
        file.write("\t\t\t\t\t\t\tNULL);\n")
        file.write("};\n")
        file.write("\n")

def generateFunctionHeader(fName : str):
    global sourceFile
    with open(sourceFile, "a") as file:
        file.write("/******************************************************************************\n")
        file.write("* Function : {}\n".format(fName))
        file.write("*//** \n")
        file.write("* \\b Description:\n")
        file.write("*\n")
        file.write("* This function TODO\n")
        file.write("*\n")
        file.write("* PRE-CONDITION: None\n")
        file.write("*\n")
        file.write("* POST-CONDITION: None\n")
        file.write("*\n")
        file.write("* @param    a: (IN)TODO\n")
        file.write("*\n")
        file.write("* @return   None\n")
        file.write("*\n")
        file.write("*******************************************************************************/\n")

def generateRunFunction(smName):
    global sourceFile
    with open(sourceFile, "a") as file:
        file.write("void Run{}(void)\n".format(smName))
        file.write("{\n")
        file.write("\tezmStateMachine_Execution(&st{});\n".format(smName))
        file.write("};\n")
        file.write("\n")

def generateStateFunctionBody(fName : str, stateDict : dict):
    global sourceFile
    generateFunctionHeader(fName)
    with open(sourceFile, "a") as file:
        file.write("/* {} section *****************************************************/\n\n".format(fName.upper()))
        file.write("static uint8_t {} (uint8_t eEvent, void * pstData)\n".format(fName))
        file.write("{\n")
        file.write("\tuint8_t u8NextState = STATE_ERROR;\n\n")
        
        file.write("\tswitch(eEvent)\n")
        file.write("\t{\n")
        for key, value in stateDict.items():
            file.write("\tcase {}:\n".format(key.upper()))
            file.write("\t{\n")
            file.write("\t\tu8NextState = {};\n".format(value[1].upper()))
            file.write("\t\tbreak;\n")
            file.write("\t}\n")
            file.write("\n")

        file.write("\tdefault:\n")
        file.write("\t{\n")
        file.write("\t\tbreak;\n")
        file.write("\t}\n")
        file.write("\n")

        file.write("\t}\n")
        file.write("\treturn u8NextState;\n")
        file.write("};\n")
        
        file.write("\n")
    
def generateEntryExitFunction(entryfName : str, exitfName : str):
    global sourceFile
    generateFunctionHeader(entryfName)
    with open(sourceFile, "a") as file:
        file.write("static void {}(void)\n".format(entryfName))
        file.write("{\n")
        file.write("}\n\n")

    generateFunctionHeader(exitfName)
    with open(sourceFile, "a") as file:
        file.write("static void {}(void)\n".format(exitfName))
        file.write("{\n")
        file.write("}\n\n")

def generateFunctions():
    index = 0
    for state in stateList:
        if index > len(stateList):
            logging.error("generate function error")
            return

        generateStateFunctionBody(stateHandlingFunctionLists[index][0], state)
        generateEntryExitFunction(stateHandlingFunctionLists[index][1], stateHandlingFunctionLists[index][2])
        index = index + 1

def generateOtherFunction():
    global sourceFile
    generateFunctionHeader("HandleStateMachineError")
    with open(sourceFile, "a") as file:
        file.write("static void HandleStateMachineError(void)\n")
        file.write("{\n")
        file.write("}\n\n")

    generateFunctionHeader("GetExternalEvent")
    with open(sourceFile, "a") as file:
        file.write("static uint8_t GetExternalEvent (void)\n")
        file.write("{\n")
        file.write("}\n\n")

def includeHeader():
    global sourceFile
    with open(sourceFile, "a") as file:
        file.write("#include \"{}\"\n\n".format(headerFile))

def generateHeaderFile(smName : str):
    global headerFile
    with open(headerFile, "a") as file:
        #remove the last  characters
        modHeaderName = headerFile[:len(headerFile) - 2] 
        
        guard_string = "_{}_H".format(modHeaderName.upper())
        
        file.write("#ifndef {}\n".format(guard_string))
        file.write("#define {}\n\n".format(guard_string))
        file.write("void Init{}(void);\n".format(smName))
        file.write("void Run{}(void);\n\n".format(smName))
        file.write("#endif /*{}*/\n".format(guard_string))
        file.write("/* End of file */\n")

def generateSourceFile():
    stateMachineName = getSmName()
    includeHeader()
    generateTypedefEnum(eventNames, True)
    generateTypedefEnum(stateNames, False)
    generateStatemachineStruct(stateMachineName)
    generateStateHandle(stateNames)
    generateStateArray(stateNames, stateHandlingFunctionLists)
    generateInitFunction(stateMachineName)
    generateRunFunction(stateMachineName)
    generateOtherFunction()
    generateFunctions()

def generateXmlTemplate(xmlName : str):
    root = ET.Element("Statemachine", name = "{}".format(xmlName))
    tree = ET.ElementTree(root)
    state = ET.SubElement(root, "State", name = "SampleState")
    event = ET.SubElement(state, "Event", name = "SampleEvent")
    ET.SubElement(event, "Function").text = "SampleFunction"
    ET.SubElement(event, "NextState").text = "SampleNextState"
    tree.write("{}.xml".format(xmlName))

def parseXmlData():
    getEvent()
    getState()

def main():
    global sourceFile
    global headerFile

    my_parser.add_argument('--output', 
                            action='store', 
                            type=str, 
                            help='name of the source and header file without .c or .h')
    
    my_parser.add_argument('--xmlPath', 
                            action='store', 
                            type=str, 
                            help='xml file contain the statemachine metadata')
    
    my_parser.add_argument('--command', 
                            action='store', 
                            type=str, 
                            required=True,
                            help='command to generate xml file and state machine')
    
    my_parser.add_argument('--smName', 
                            action='store', 
                            type=str, 
                            help='Name of the state machine')

    args  = my_parser.parse_args()

    if args.command == "xml":
        smName = args.smName
        if smName == None:
            logging.error("name is empty")
            return
        else:
            generateXmlTemplate(smName)
       
    elif args.command == "statemachine":
        if args.output == None or args.xmlPath == None:
            logging.error("input is empty")
            return
        else:
            headerFile = args.output + ".h"
            sourceFile = args.output + ".c"

            if not os.path.exists(args.xmlPath):
                logging.error("no XML file found")
                return

            if os.path.exists(headerFile):
                os.remove(headerFile)

            if os.path.exists(sourceFile):
                os.remove(sourceFile)

            tree = ET.parse(args.xmlPath)
            global root
            root = tree.getroot()
            
            stateMachineName = getSmName()
            parseXmlData()
            generateHeaderFile(stateMachineName)
            generateSourceFile()
    else:
        #do nothing
        pass
    

if __name__ == "__main__":
    # execute only if run as a script
    main()