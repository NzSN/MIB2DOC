//
// Created by ayden on 2017/4/19.
//

#include "../include/type.h"

/* Declaration of static functions define in this file */
static int nameOp(MIB_NODE_INFO *nodeInfo);
static int objSpecOp(MIB_NODE_INFO *nodeInfo);
static int objValOp(MIB_NODE_INFO *nodeInfo);
static int synSpecOp(MIB_NODE_INFO *nodeInfo);
static int synValOp(MIB_NODE_INFO *nodeInfo);
static int accessSpecOp(MIB_NODE_INFO *nodeInfo);
static int accessValOp(MIB_NODE_INFO *nodeInfo);
static int statusSpecOp(MIB_NODE_INFO *nodeInfo);
static int statusValOp(MIB_NODE_INFO *nodeInfo);
static int descSpecOp(MIB_NODE_INFO *nodeInfo);
static int descValOp(MIB_NODE_INFO *nodeInfo);
static int mountSpecOp(MIB_NODE_INFO *nodeInfo);
static int mountValOp(MIB_NODE_INFO *nodeInfo);

int tokenOperation(int token, MIB_NODE_INFO *nodeInfo) {
    int ret = 0;

    switch (token) {
        case IDENTIFIER:
            ret = nameOp(nodeInfo);
            break;
        case OBJ_SPECIFIER:
            ret = objSpecOp(nodeInfo);
            break;
        case SYNTAX_SPECIFIER:
            ret = synSpecOp(nodeInfo);
            break;
        case ACCESS_SPECIFIER:
            ret = accessSpecOp(nodeInfo);
            break;
        case ACCESS_VALUE:
            ret = accessValOp(nodeInfo);
            break;
        case STATUS_SPECIFIER:
            ret = statusSpecOp(nodeInfo);
            break;
        case STATUS_VALUE:
            ret = statusValOp(nodeInfo);
            break;
        case DESC_SPECIFIER:
            ret = descSpecOp(nodeInfo);
            break;
        case DESC_VALUE:
            ret = descValOp(nodeInfo);
            break;
        case ASSIGNED:
            ret = mountSpecOp(nodeInfo);
            break;
        case MOUNT_POINT:
            ret = mountValOp(nodeInfo);
            break;
        default:
            ret = -1;
            break;
    }
    return ret;
}

static int nameOp(MIB_NODE_INFO *nodeInfo) {}
static int objSpecOp(MIB_NODE_INFO *nodeInfo) {}
static int objValOp(MIB_NODE_INFO *nodeInfo) {}
static int synSpecOp(MIB_NODE_INFO *nodeInfo) {}
static int synValOp(MIB_NODE_INFO *nodeInfo) {}
static int accessSpecOp(MIB_NODE_INFO *nodeInfo) {}
static int accessValOp(MIB_NODE_INFO *nodeInfo) {}
static int statusSpecOp(MIB_NODE_INFO *nodeInfo) {}
static int statusValOp(MIB_NODE_INFO *nodeInfo) {}
static int descSpecOp(MIB_NODE_INFO *nodeInfo) {}
static int descValOp(MIB_NODE_INFO *nodeInfo) {}
static int mountSpecOp(MIB_NODE_INFO *nodeInfo) {}
static int mountValOp(MIB_NODE_INFO *nodeInfo) {}

/* Functions convert info in MIB_NODE_INFO to LaTex code */
int fromMibToLaTex(MIB_NODE_INFO *nodeinfo) {

}