#ifndef <%= component.name %>_INTERMEDIATES_HH
#define <%= component.name %>_INTERMEDIATES_HH

#include <boost/cstdint.hpp>
#include <<%= component.name %>ToolkitTypes.hpp>
<% if toolkit.has_opaques_with_templates? %>
#include <<%= component.name %>ToolkitUser.hpp>
<% end %>

namespace <%= component.name %>
{
    <% toolkit.opaques.find_all { |op| !op.generate_templates? }.each do |opaque_def|
        from = opaque_def.type
        into = component.find_type(opaque_def.intermediate)
        if opaque_def.needs_copy? %>
    void to_intermediate(<%= into.cxx_name %>& intermediate, <%= from.cxx_name %> const& real_type);
    void from_intermediate(<%= from.cxx_name %>& real_type, <%= into.cxx_name %>& intermediate);
        <% else %>
    <%= into.cxx_name %> const& to_intermediate(<%= from.cxx_name %> const& real_type);
    bool from_intermediate(<%= from.cxx_name %>& real_type, <%= into.cxx_name %>* intermediate);
        <% end %>
    <% end %>

<%
opaques.find_all { |opdef| !opdef.needs_copy? }.each do |opdef|
    type = opdef.type
    intermediate_type = component.find_type(opdef.intermediate)
%>
    void from_intermediate(<%= type.ref_type %> value, <%= intermediate_type.arg_type %> _intermediate);
<% end %>

<%
generated_types.
    find_all { |t| t.contains_opaques? && !t.opaque? }.
    each do |type|
        m_type = toolkit.find_type(type.name + "_m")
%>
    void to_intermediate(<%= m_type.ref_type %> intermediate, <%= type.arg_type %> value);
    void from_intermediate(<%= type.ref_type %> value, <%= m_type.ref_type %> intermediate);
<% end %>
}

#endif
