#include "ProfileMgr.h"
#include <BlackBone/src/BlackBone/Misc/Utils.h>

#ifdef USE64
        #define CURRENT_PROFILE L"\\64.xml"
    #else
        #define CURRENT_PROFILE L"\\32.xml"
#endif

bool ProfileMgr::Save( const std::wstring& path /*= L""*/)
{
    try
    {
        auto filepath = path.empty() ? (blackbone::Utils::GetExeDirectory() + CURRENT_PROFILE) : path;

        acut::XmlDoc<wchar_t> xml;
        xml.create_document();

        for (auto& imgpath : _config.images)
            xml.append( L".path" ).value( imgpath );

        xml.set( L".1", _config.mmapFlags );
        xml.set( L".2",       _config.procName.c_str() );
        xml.set( L".3",         _config.hijack );
        xml.set( L".4",         _config.unlink );
        xml.set( L".5",        _config.erasePE );
        xml.set( L".6",          _config.close );
        xml.set( L".7",       _config.injIndef );
        xml.set( L".8",    _config.processMode );
        xml.set( L".9",     _config.injectMode );
        xml.set( L".10",          _config.delay );
        xml.set( L".11",         _config.period );
        xml.set( L".12",           _config.skipProc );
        xml.set( L".13",    _config.procCmdLine.c_str() );
        xml.set( L".14",    _config.initRoutine.c_str() );
        xml.set( L".15",       _config.initArgs.c_str() );

        xml.write_document( filepath );
        
        return true;
    }
    catch (const std::runtime_error&)
    {
        return false;
    }
}

bool ProfileMgr::Load( const std::wstring& path /*= L""*/ )
{
    try
    {
        auto filepath = path.empty() ? (blackbone::Utils::GetExeDirectory() + CURRENT_PROFILE) : path;
        if (!acut::file_exists( filepath ))
            return false;

        acut::XmlDoc<wchar_t> xml;
        xml.read_from_file( filepath );

        // Load images in a safe way
        if(xml.has( L".path" ))
        {
            auto nodes = xml.all_nodes_named( L".path" );
            for (auto& node : nodes)
                _config.images.emplace_back( node.value() );
        }

        xml.get_if_present( L".1",  _config.mmapFlags );
        xml.get_if_present( L".2",        _config.procName );
        xml.get_if_present( L".3",          _config.hijack );
        xml.get_if_present( L".4",          _config.unlink );
        xml.get_if_present( L".5",         _config.erasePE );
        xml.get_if_present( L".6",           _config.close );
        xml.get_if_present( L".7",        _config.injIndef );
        xml.get_if_present( L".8",     _config.processMode );
        xml.get_if_present( L".9",      _config.injectMode );
        xml.get_if_present( L".10",           _config.delay );
        xml.get_if_present( L".11",          _config.period );
        xml.get_if_present( L".12",            _config.skipProc );
        xml.get_if_present( L".13",     _config.procCmdLine );
        xml.get_if_present( L".14",     _config.initRoutine );
        xml.get_if_present( L".15",        _config.initArgs );

        return true;
    }
    catch (const std::runtime_error&)
    {
        return false;
    }
}
