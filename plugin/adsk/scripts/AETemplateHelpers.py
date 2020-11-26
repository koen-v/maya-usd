import ufe
import mayaUsd.ufe

def GetDefaultPrimName(proxyShape):
    if not proxyShape.startswith('|world'):
        proxyShape = '|world' + proxyShape
    try:
        proxyStage = mayaUsd.ufe.getStage(proxyShape)
        if proxyStage:
            defPrim = proxyStage.GetDefaultPrim()
            if defPrim:
                return defPrim.GetName()
    except:
        pass
    return ''

def GetRootLayerName(proxyShape):
    if not proxyShape.startswith('|world'):
        proxyShape = '|world' + proxyShape
    try:
        proxyStage = mayaUsd.ufe.getStage(proxyShape)
        if proxyStage:
            rootLayer = proxyStage.GetRootLayer()
            if rootLayer:
                return rootLayer.GetDisplayName() if rootLayer.anonymous else rootLayer.realPath
    except:
        pass
    return ''
